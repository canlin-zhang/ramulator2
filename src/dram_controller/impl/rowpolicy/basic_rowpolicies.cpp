#include "basic_rowpolicies.h"

namespace Ramulator
{
void ClosedRowPolicy::setup(IFrontEnd* frontend, IMemorySystem* memory_system)
{
    m_ctrl = cast_parent<IDRAMController>();
    m_dram = m_ctrl->m_dram;

    m_cap = param<int>("cap").default_val(10000000); // TODO

    m_rank_level = m_dram->m_levels("rank");
    m_bankgroup_level = m_dram->m_levels("bankgroup");
    m_bank_level = m_dram->m_levels("bank");
    m_row_level = m_dram->m_levels("row");

    m_PRE_req_id = m_dram->m_requests("close-row");

    m_num_ranks = m_dram->get_level_size("rank");
    m_num_bankgroups = m_dram->get_level_size("bankgroup");
    m_num_banks = m_dram->get_level_size("bank");

    m_col_accesses.resize(m_num_banks * m_num_bankgroups * m_num_ranks, 0);

    register_stat(s_num_close_reqs).name("num_close_reqs");
}

void ClosedRowPolicy::update(bool request_found, ReqBuffer::iterator& req_it)
{

    if (!request_found)
        return;

    if (m_dram->m_command_meta(req_it->command).is_closing ||
        m_dram->m_command_meta(req_it->command).is_refreshing) // PRE or REF
    {

        if (req_it->addr_vec[m_bankgroup_level] == -1 && req_it->addr_vec[m_bank_level] == -1)
        { // all bank closes
            for (int b = 0; b < m_num_banks; b++)
            {
                for (int bg = 0; bg < m_num_bankgroups; bg++)
                {
                    int rank_id = req_it->addr_vec[m_rank_level];
                    int flat_bank_id =
                        b + bg * m_num_banks + rank_id * m_num_banks * m_num_bankgroups;
                    m_col_accesses[flat_bank_id] = 0;
                }
            }
        }
        else if (req_it->addr_vec[m_bankgroup_level] == -1)
        { // same bank closes
            for (int bg = 0; bg < m_num_bankgroups; bg++)
            {
                int bank_id = req_it->addr_vec[m_bank_level];
                int rank_id = req_it->addr_vec[m_rank_level];
                int flat_bank_id =
                    bank_id + bg * m_num_banks + rank_id * m_num_banks * m_num_bankgroups;
                m_col_accesses[flat_bank_id] = 0;
            }
        }
        else
        { // single bank closes  (PRE, VRR, RDA, WRA)
            int flat_bank_id = req_it->addr_vec[m_bank_level] +
                               req_it->addr_vec[m_bankgroup_level] * m_num_banks +
                               req_it->addr_vec[m_rank_level] * m_num_banks * m_num_bankgroups;

            m_col_accesses[flat_bank_id] = 0;
        }
    }
    else if (m_dram->m_command_meta(req_it->command).is_accessing) // RD or WR
    {
        int flat_bank_id = req_it->addr_vec[m_bank_level] +
                           req_it->addr_vec[m_bankgroup_level] * m_num_banks +
                           req_it->addr_vec[m_rank_level] * m_num_banks * m_num_bankgroups;

        m_col_accesses[flat_bank_id]++;

        if (m_col_accesses[flat_bank_id] >= m_cap)
        {
            Request req(req_it->addr_vec, m_PRE_req_id);
            m_ctrl->priority_send(req);
            m_col_accesses[flat_bank_id] = 0;
            s_num_close_reqs++;
        }
    }
}

} // namespace Ramulator