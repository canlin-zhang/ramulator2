#include "blocking_scheduler.h"

namespace Ramulator
{

void BlockingScheduler::setup(IFrontEnd* frontend, IMemorySystem* memory_system)
{
    m_dram = cast_parent<IBHDRAMController>()->m_dram;
    m_bh = cast_parent<IBHDRAMController>()->get_plugin<IBlockHammer>();
    if (!m_bh)
    {
        std::cout << "BlockHammer scheduler requires BlockHammer plugin enabled!" << std::endl;
        std::exit(0);
    }
}

ReqBuffer::iterator BlockingScheduler::compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2)
{
    bool ready1 = m_dram->check_ready(req1->command, req1->addr_vec);
    bool ready2 = m_dram->check_ready(req2->command, req2->addr_vec);

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

ReqBuffer::iterator BlockingScheduler::get_best_request(ReqBuffer& buffer)
{
    if (buffer.size() == 0)
    {
        return buffer.end();
    }

    for (auto& req : buffer)
    {
        req.command = m_dram->get_preq_command(req.final_command, req.addr_vec);
    }

    auto candidate = buffer.begin();
    while (candidate != buffer.end() && !m_bh->is_act_safe(*candidate))
    {
        candidate++;
    }

    if (candidate == buffer.end())
    {
        return buffer.end();
    }

    // std::next(candidate, 1)
    for (auto next = std::next(buffer.begin(), 1); next != buffer.end(); next++)
    {
        if (!m_bh->is_act_safe(*next))
        {
            continue;
        }
        candidate = compare(candidate, next);
    }
    return candidate;
}

} // namespace Ramulator
