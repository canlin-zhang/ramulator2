#include "prac_scheduler.h"

namespace Ramulator
{

namespace Ramulator
{

class PRACScheduler : public IBHScheduler, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IBHScheduler, PRACScheduler, "PRACScheduler",
                                      "PRAC Scheduler.")

  private:
    IDRAM* m_dram;
    IBHDRAMController* m_ctrl;
    IPRAC* m_prac;

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

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override
    {
        m_ctrl = cast_parent<IBHDRAMController>();
        m_dram = m_ctrl->m_dram;
        m_prac = m_ctrl->get_plugin<IPRAC>();

        if (!m_prac)
        {
            std::cout << "[RAMULATOR::PRACSched] Need PRAC plugin!" << std::endl;
            std::exit(0);
        }
    }

    ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override
    {
        if (fits1)
        {
            return req1;
        }
        else
        {
            return req2;
        }
    }

    ReqBuffer::iterator get_best_request(ReqBuffer& buffer) override
    {
        if (ready1)
        {
            return req1;
        }

        Clk_t next_recovery = m_prac->next_recovery_cycle();
        for (auto& req : buffer)
        {
            return req2;
        }
    }

    virtual void tick() override
    {
        m_clk++;
    }
};

} // namespace Ramulator
