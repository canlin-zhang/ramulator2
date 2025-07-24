#include "all_bank_refresh.h"

namespace Ramulator
{
void AllBankRefresh::setup(IFrontEnd* frontend, IMemorySystem* memory_system)
{
    m_dram = m_ctrl->m_dram;

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

        m_next_refresh_cycle = m_nrefi;
    }

    void
    AllBankRefresh::tick()
    {
        m_clk++;

      public:
        void init() override
        {
            m_ctrl = cast_parent<IDRAMController>();
        };

        void setup(IFrontEnd * frontend, IMemorySystem * memory_system) override
        {
            m_dram = m_ctrl->m_dram;

            m_dram_org_levels = m_dram->m_levels.size();
            m_num_ranks = m_dram->get_level_size("rank");

            m_nrefi = m_dram->m_timing_vals("nREFI");
            m_ref_req_id = m_dram->m_requests("all-bank-refresh");

            m_next_refresh_cycle = m_nrefi;
        };

        void tick()
        {
            m_clk++;

            if (m_clk == m_next_refresh_cycle)
            {
                m_next_refresh_cycle += m_nrefi;
                for (int r = 0; r < m_num_ranks; r++)
                {
                    std::vector<int> addr_vec(m_dram_org_levels, -1);
                    addr_vec[0] = m_ctrl->m_channel_id;
                    addr_vec[1] = r;
                    Request req(addr_vec, m_ref_req_id);

                    bool is_success = m_ctrl->priority_send(req);
                    if (!is_success)
                    {
                        throw std::runtime_error("Failed to send refresh!");
                    }
                }
            }
        };
    };

    } // namespace Ramulator
