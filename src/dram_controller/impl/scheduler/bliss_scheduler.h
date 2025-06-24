#pragma once
#include <vector>

#include "base.h"
#include "bh_controller.h"
#include "bh_scheduler.h"
#include "impl/plugin/bliss/bliss.h"

namespace Ramulator
{

    class BLISSScheduler : public IBHScheduler, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IBHScheduler, BLISSScheduler, "BLISS", "BLISS Scheduler.")

    private:
        IDRAM *m_dram;
        IBLISS *m_bliss;

        int m_clk = -1;

        int m_req_rd = -1;
        int m_req_wr = -1;

        bool m_is_debug;

        const int SAFE_IDX = 0;
        const int READY_IDX = 1;

    public:
        void init() override {}

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override;

        ReqBuffer::iterator get_best_request(ReqBuffer &buffer) override;

        virtual void tick() override { m_clk++; }
    };

} // namespace Ramulator
