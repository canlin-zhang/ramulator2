#pragma once
#include <vector>

#include "base.h"
#include "controller.h"
#include "refresh.h"

namespace Ramulator
{

class AllBankRefresh : public IRefreshManager, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IRefreshManager, AllBankRefresh, "AllBank",
                                      "All-Bank Refresh scheme.")
  private:
    Clk_t m_clk = 0;
    IDRAM* m_dram;
    IDRAMController* m_ctrl;

    int m_dram_org_levels = -1;
    int m_num_ranks = -1;

    int m_nrefi = -1;
    int m_ref_req_id = -1;
    Clk_t m_next_refresh_cycle = -1;

  public:
    void init() override
    {
        m_ctrl = cast_parent<IDRAMController>();
    };

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;

    void tick();
};

} // namespace Ramulator
