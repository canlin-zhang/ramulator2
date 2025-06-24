#pragma once
#include <vector>

#include "base.h"
#include "bh_controller.h"
#include "bh_scheduler.h"
#include "impl/plugin/blockhammer/blockhammer.h"

namespace Ramulator
{

class BlockingScheduler : public IBHScheduler, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IBHScheduler, BlockingScheduler, "BlockingScheduler",
                                      "Blocking DRAM Scheduler.")

  private:
    IDRAM* m_dram;
    IBlockHammer* m_bh;

    int m_clk = -1;

    // stats
    int s_num_blacklist = 0;

    bool m_is_debug;

  public:
    void init() override
    {
    }

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;

    ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override;

    ReqBuffer::iterator get_best_request(ReqBuffer& buffer) override;

    virtual void tick() override
    {
        m_clk++;
    }
};

} // namespace Ramulator
