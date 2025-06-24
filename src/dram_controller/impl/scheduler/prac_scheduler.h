#pragma once
#include <vector>

#include "base.h"
#include "bh_controller.h"
#include "bh_scheduler.h"
#include "impl/plugin/prac/prac.h"

namespace Ramulator
{

    class PRACScheduler : public IBHScheduler, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IBHScheduler, PRACScheduler, "PRACScheduler", "PRAC Scheduler.")

    private:
        IDRAM *m_dram;
        IBHDRAMController *m_ctrl;
        IPRAC *m_prac;

        std::unordered_map<int, int> lut_cycles_needed;

        Clk_t m_clk = 0;

        bool m_is_debug = false;

        const int FITS_IDX = 0;
        const int READY_IDX = 1;

    public:
        void init() override
        {
            m_is_debug = param<bool>("debug").default_val(false);
        }

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override;

        ReqBuffer::iterator get_best_request(ReqBuffer &buffer) override;

        virtual void tick() override { m_clk++; }
    };

} // namespace Ramulator
