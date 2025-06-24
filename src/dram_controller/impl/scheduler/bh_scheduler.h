#pragma once
#include <vector>

#include "base.h"
#include "bh_controller.h"
#include "bh_scheduler.h"

namespace Ramulator
{

    class BHScheduler : public IBHScheduler, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IBHScheduler, BHScheduler, "BHScheduler", "BHammer Scheduler.")

    private:
        IDRAM *m_dram;

        int m_clk = -1;

        bool m_is_debug;

    public:
        void init() override
        {
        }

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override
        {
            m_dram = cast_parent<IDRAMController>()->m_dram;
        }

        ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override;

        ReqBuffer::iterator get_best_request(ReqBuffer &buffer) override;

        virtual void tick() override
        {
            m_clk++;
        }
    };

} // namespace Ramulator
