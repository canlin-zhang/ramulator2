#include "bh_dram_controller.h"

namespace Ramulator
{
void BHDRAMController::init()
{
    m_invalidate_ctr = 0;
    m_wr_low_watermark = param<float>("wr_low_watermark")
                             .desc("Threshold for switching back to read mode.")
                             .default_val(0.2f);
    m_wr_high_watermark = param<float>("wr_high_watermark")
                              .desc("Threshold for switching to write mode.")
                              .default_val(0.8f);

    m_scheduler = create_child_ifce<IBHScheduler>();
    m_refresh = create_child_ifce<IRefreshManager>();
    m_rowpolicy = create_child_ifce<IRowPolicy>();
    m_logger = Logging::create_logger("DBHCTRL");

    if (m_config["plugins"])
    {
        YAML::Node plugin_configs = m_config["plugins"];
        for (YAML::iterator it = plugin_configs.begin(); it != plugin_configs.end(); ++it)
        {
            m_plugins.push_back(create_child_ifce<IControllerPlugin>(*it));
        }
    }
}

void BHDRAMController::setup(IFrontEnd* frontend, IMemorySystem* memory_system)
{
    m_llc = static_cast<BHO3*>(frontend)->get_llc();
    m_dram = memory_system->get_ifce<IDRAM>();
    m_rank_addr_idx = m_dram->m_levels("rank");
    m_bankgroup_addr_idx = m_dram->m_levels("bankgroup");
    m_bank_addr_idx = m_dram->m_levels("bank");
    m_row_addr_idx = m_dram->m_levels("row");
    m_priority_buffer.max_size = 512 * 3 + 32;

    int num_cores = static_cast<BHO3*>(frontend)->get_num_cores();
    s_core_row_hits.resize(num_cores);
    s_core_row_misses.resize(num_cores);
    s_core_row_conflicts.resize(num_cores);

    for (int i = 0; i < num_cores; i++)
    {
        register_stat(s_core_row_hits[i]).name("controller_core_row_hits_{}", i);
        register_stat(s_core_row_misses[i]).name("controller_core_row_misses_{}", i);
        register_stat(s_core_row_conflicts[i]).name("controller_core_row_conflicts_{}", i);
    }

    m_priority_buffer.max_size = std::numeric_limits<size_t>::max();

    register_stat(s_num_row_hits).name("controller_num_row_hits");
    register_stat(s_num_row_misses).name("controller_num_row_misses");
    register_stat(s_num_row_conflicts).name("controller_num_row_conflicts");
}

bool BHDRAMController::send(Request& req)
{
    req.final_command = m_dram->m_request_translations(req.type_id);

    // Forward existing write requests to incoming read requests
    if (req.type_id == Request::Type::Read)
    {
        auto compare_addr = [req](const Request& wreq) { return wreq.addr == req.addr; };
        if (std::find_if(m_write_buffer.begin(), m_write_buffer.end(), compare_addr) !=
            m_write_buffer.end())
        {
            // The request will depart at the next cycle
            req.depart = m_clk + 1;
            pending.push_back(req);
            return true;
        }
    }

    // Else, enqueue them to corresponding buffer based on request type id
    bool is_success = false;
    req.arrive = m_clk;
    if (req.type_id == Request::Type::Read)
    {
        is_success = m_read_buffer.enqueue(req);
    }
    else if (req.type_id == Request::Type::Write)
    {
        is_success = m_write_buffer.enqueue(req);
    }
    else
    {
        throw std::runtime_error("Invalid request type!");
    }
    if (!is_success)
    {
        // We could not enqueue the request
        req.arrive = -1;
        return false;
    }

    return true;
}

bool BHDRAMController::priority_send(Request& req)
{
    req.final_command = m_dram->m_request_translations(req.type_id);

    bool is_success = false;
    is_success = m_priority_buffer.enqueue(req);
    return is_success;
}

void BHDRAMController::tick()
{
    m_clk++;
    // 1. Serve completed reads
    serve_completed_reads();

    m_refresh->tick();
    m_scheduler->tick();

    // 2. Try to find a request to serve.
    ReqBuffer::iterator req_it;
    ReqBuffer* buffer = nullptr;
    bool request_found = schedule_request(req_it, buffer);

    // 2.1 RowPolicy
    m_rowpolicy->update(request_found, req_it);

    // 3. Update all plugins
    for (auto plugin : m_plugins)
    {
        plugin->update(request_found, req_it);
    }

    // 4. Finally, issue the commands to serve the request
    if (request_found)
    {
        // If we find a real request to serve
        m_dram->issue_command(req_it->command, req_it->addr_vec);

        // If we are issuing the last command, set depart clock cycle and move the request to the
        // pending queue
        if (req_it->command == req_it->final_command)
        {
            if (req_it->type_id == Request::Type::Read)
            {
                req_it->depart = m_clk + m_dram->m_read_latency;
                pending.push_back(*req_it);
            }
            else if (req_it->type_id == Request::Type::Write)
            {
                // TODO: Add code to update statistics
            }
            buffer->remove(req_it);
        }
        else
        {
            if (m_dram->m_command_meta(req_it->command).is_opening)
            {
                if (m_active_buffer.enqueue(*req_it))
                {
                    buffer->remove(req_it);
                }
            }
        }
    }
}

void BHDRAMController::serve_completed_reads()
{
    if (pending.size())
    {
        // Check the first pending request
        auto& req = pending[0];
        if (req.depart <= m_clk)
        {
            // Request received data from dram
            if (req.depart - req.arrive > 1)
            {
                // Check if this requests accesses the DRAM or is being forwarded.
                // TODO add the stats back
            }

            if (req.callback)
            {
                // If the request comes from outside (e.g., processor), call its callback
                req.callback(req);
            }
            // Finally, remove this request from the pending queue
            pending.pop_front();
        }
    };
}

void BHDRAMController::set_write_mode()
{
    if (!m_is_write_mode)
    {
        if ((m_write_buffer.size() > m_wr_high_watermark * m_write_buffer.max_size) ||
            m_read_buffer.size() == 0)
        {
            m_is_write_mode = true;
        }
    }
    else
    {
        if ((m_write_buffer.size() < m_wr_low_watermark * m_write_buffer.max_size) &&
            m_read_buffer.size() != 0)
        {
            m_is_write_mode = false;
        }
    }
}

bool BHDRAMController::schedule_request(ReqBuffer::iterator& req_it, ReqBuffer*& req_buffer)
{
    bool request_found = false;
    // 2.1    First, check the act buffer to serve requests that are already activating (avoid
    // useless ACTs)
    if (req_it = m_scheduler->get_best_request(m_active_buffer); req_it != m_active_buffer.end())
    {
        if (m_dram->check_ready(req_it->command, req_it->addr_vec))
        {
            request_found = true;
            req_buffer = &m_active_buffer;
        }
    }
    // 2.2    If no requests can be scheduled from the act buffer, check the rest of the buffers
    if (!request_found)
    {
        // 2.2.1    We first check the priority buffer to prioritize e.g., maintenance requests
        if (m_priority_buffer.size() != 0)
        {
            req_buffer = &m_priority_buffer;
            req_it = m_priority_buffer.begin();
            req_it->command = m_dram->get_preq_command(req_it->final_command, req_it->addr_vec);

            request_found = m_dram->check_ready(req_it->command, req_it->addr_vec);
            if (!request_found & m_priority_buffer.size() != 0)
            {
                return false;
            }
        }

        // 2.2.1    If no request to be scheduled in the priority buffer, check the read and write
        // buffers.
        if (!request_found)
        {
            // Query the write policy to decide which buffer to serve
            set_write_mode();
            auto& buffer = m_is_write_mode ? m_write_buffer : m_read_buffer;
            if (req_it = m_scheduler->get_best_request(buffer); req_it != buffer.end())
            {
                request_found = m_dram->check_ready(req_it->command, req_it->addr_vec);
                req_buffer = &buffer;
            }
        }
    }

    if (request_found)
    {
        if (m_dram->m_command_meta(req_it->command).is_closing)
        {
            auto& rowgroup = req_it->addr_vec;
            for (auto _it = m_active_buffer.begin(); _it != m_active_buffer.end(); _it++)
            {
                auto& _it_rowgroup = _it->addr_vec;
                bool is_matching = true;
                for (int i = 0; i < m_bank_addr_idx + 1; i++)
                {
                    if (_it_rowgroup[i] != rowgroup[i] && _it_rowgroup[i] != -1 &&
                        rowgroup[i] != -1)
                    {
                        is_matching = false;
                        break;
                    }
                }
                if (is_matching)
                {
                    request_found = false;
                    break;
                }
            }
        }
    }

    if (request_found && req_buffer != &m_active_buffer)
    {
        if (req_it->type_id == Request::Type::Read || req_it->type_id == Request::Type::Write)
        {
            auto& req_meta = m_dram->m_command_meta(req_it->command);
            int source_id = req_it->source_id >= 0 ? req_it->source_id : 0;
            int increment = req_it->source_id >= 0 ? 1 : 0;
            if (req_meta.is_accessing)
            {
                s_core_row_hits[source_id] += increment;
                s_num_row_hits++;
            }
            if (req_meta.is_opening)
            {
                s_core_row_misses[source_id] += increment;
                s_num_row_misses++;
            }
            if (req_meta.is_closing)
            {
                s_core_row_conflicts[source_id] += increment;
                s_num_row_conflicts++;
            }
        }
    }
    return request_found;
}

} // namespace Ramulator