#pragma once
#include "bh_controller.h"
#include "frontend.h"
#include "impl/processor/bhO3/bhO3.h"
#include "impl/processor/bhO3/bhllc.h"
#include "memory_system.h"

namespace Ramulator
{

DECLARE_DEBUG_FLAG(DBHCTRL);
ENABLE_DEBUG_FLAG(DBHCTRL);

class BHDRAMController final : public IBHDRAMController, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IBHDRAMController, BHDRAMController, "BHDRAMController",
                                      "BHammer DRAM controller.");

  private:
    Logger_t m_logger;
    std::deque<Request>
        pending; // A queue for read requests that are about to finish (callback after RL)
    BHO3LLC* m_llc;

    ReqBuffer m_active_buffer; // Buffer for requests being served. This has the highest priority
    ReqBuffer
        m_priority_buffer;    // Buffer for high-priority requests (e.g., maintenance like refresh).
    ReqBuffer m_read_buffer;  // Read request buffer
    ReqBuffer m_write_buffer; // Write request buffer

    int m_rank_addr_idx = -1;
    int m_bankgroup_addr_idx = -1;
    int m_bank_addr_idx = -1;
    int m_row_addr_idx = -1;

    float m_wr_low_watermark;
    float m_wr_high_watermark;
    bool m_is_write_mode = false;

    std::vector<int> s_core_row_hits;
    std::vector<int> s_core_row_misses;
    std::vector<int> s_core_row_conflicts;

    int s_num_row_hits = 0;
    int s_num_row_misses = 0;
    int s_num_row_conflicts = 0;

    // DEBUG STAT
    int m_invalidate_ctr = -1;

  public:
    void init() override;

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;

    bool send(Request& req) override;

    bool priority_send(Request& req) override;

    void tick() override;

  private:
    /**
     * @brief    Helper function to serve the completed read requests
     * @details
     * This function is called at the beginning of the tick() function.
     * It checks the pending queue to see if the top request has received data from DRAM.
     * If so, it finishes this request by calling its callback and poping it from the pending queue.
     */
    void serve_completed_reads();

    /**
     * @brief    Checks if we need to switch to write mode
     *
     */
    void set_write_mode();

    /**
     * @brief    Helper function to find a request to schedule from the buffers.
     *
     */
    bool schedule_request(ReqBuffer::iterator& req_it, ReqBuffer*& req_buffer);

    void finalize() override
    {
    }
};
} // namespace Ramulator
