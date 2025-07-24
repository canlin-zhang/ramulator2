#include "bliss_scheduler.h"

namespace Ramulator
{

void BLISSScheduler::setup(IFrontEnd* frontend, IMemorySystem* memory_system)
{
    auto* ctrl = cast_parent<IBHDRAMController>();
    m_dram = ctrl->m_dram;
    m_bliss = ctrl->get_plugin<IBLISS>();

    m_req_rd = m_dram->m_requests("read");
    m_req_wr = m_dram->m_requests("write");

    if (!m_bliss)
    {
        throw ConfigurationError(
            "[Ramulator::BLISSScheduler] Implementation requires BLISS plugin to be active.");
    }
}

ReqBuffer::iterator BLISSScheduler::compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2)
{
    bool safe1 = req1->scratchpad[SAFE_IDX];
    bool safe2 = req2->scratchpad[SAFE_IDX];

    if (safe1 ^ safe2)
    {
        if (safe1)
        {
            return req1;
        }
        else
        {
            return req2;
        }
    }

    bool ready1 = req1->scratchpad[READY_IDX];
    bool ready2 = req2->scratchpad[READY_IDX];

    if (ready1 ^ ready2)
    {
        if (ready1)
        {
            return req1;
        }
        else
        {
            return req2;
        }
    }

    // Fallback to FCFS
    if (req1->arrive <= req2->arrive)
    {
        return req1;
    }
    else
    {
        return req2;
    }
}

ReqBuffer::iterator BLISSScheduler::get_best_request(ReqBuffer& buffer)
{
    if (buffer.size() == 0)
    {
        return buffer.end();
    }

    for (auto& req : buffer)
    {
        req.command = m_dram->get_preq_command(req.final_command, req.addr_vec);

        // Check if the request is safe to issue
        bool blisted = m_bliss->is_blacklisted(req.source_id);
        bool isrw = req.type_id == m_req_rd || req.type_id == m_req_wr;
        bool safe = !isrw || !blisted;
        req.scratchpad[SAFE_IDX] = safe;

        // Check if the request is ready
        bool ready = m_dram->check_ready(req.command, req.addr_vec);
        req.scratchpad[READY_IDX] = ready;
    }

    auto candidate = buffer.begin();
    for (auto next = std::next(buffer.begin(), 1); next != buffer.end(); next++)
    {
        candidate = compare(candidate, next);
    }
    return candidate;
}

} // namespace Ramulator
