#pragma once
#include "controller.h"
#include "memory_system.h"

namespace Ramulator
{

    class GenericDRAMController final : public IDRAMController, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IDRAMController, GenericDRAMController, "Generic", "A generic DRAM controller.");

    private:
        std::deque<Request> pending; // A queue for read requests that are about to finish (callback after RL)

        ReqBuffer m_active_buffer;   // Buffer for requests being served. This has the highest priority
        ReqBuffer m_priority_buffer; // Buffer for high-priority requests (e.g., maintenance like refresh).
        ReqBuffer m_read_buffer;     // Read request buffer
        ReqBuffer m_write_buffer;    // Write request buffer

        int m_bank_addr_idx = -1;

        float m_wr_low_watermark;
        float m_wr_high_watermark;
        bool m_is_write_mode = false;

        size_t s_row_hits = 0;
        size_t s_row_misses = 0;
        size_t s_row_conflicts = 0;
        size_t s_read_row_hits = 0;
        size_t s_read_row_misses = 0;
        size_t s_read_row_conflicts = 0;
        size_t s_write_row_hits = 0;
        size_t s_write_row_misses = 0;
        size_t s_write_row_conflicts = 0;

        size_t m_num_cores = 0;
        std::vector<size_t> s_read_row_hits_per_core;
        std::vector<size_t> s_read_row_misses_per_core;
        std::vector<size_t> s_read_row_conflicts_per_core;

        size_t s_num_read_reqs = 0;
        size_t s_num_write_reqs = 0;
        size_t s_num_other_reqs = 0;
        size_t s_queue_len = 0;
        size_t s_read_queue_len = 0;
        size_t s_write_queue_len = 0;
        size_t s_priority_queue_len = 0;
        float s_queue_len_avg = 0;
        float s_read_queue_len_avg = 0;
        float s_write_queue_len_avg = 0;
        float s_priority_queue_len_avg = 0;

        size_t s_read_latency = 0;
        float s_avg_read_latency = 0;

    public:
        void init() override;

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        bool send(Request &req) override;

        bool priority_send(Request &req) override
        {
            req.final_command = m_dram->m_request_translations(req.type_id);

            bool is_success = false;
            is_success = m_priority_buffer.enqueue(req);
            return is_success;
        }

        void tick() override;

    private:
        /**
         * @brief    Helper function to check if a request is hitting an open row
         * @details
         *
         */
        bool is_row_hit(ReqBuffer::iterator &req);

        /**
         * @brief    Helper function to check if a request is opening a row
         * @details
         *
         */
        bool is_row_open(ReqBuffer::iterator &req);

        /**
         * @brief
         * @details
         *
         */
        void update_request_stats(ReqBuffer::iterator &req);

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
        bool schedule_request(ReqBuffer::iterator &req_it, ReqBuffer *&req_buffer);

        void finalize() override;
    };

} // namespace Ramulator
