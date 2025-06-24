#pragma once
#include <vector>

#include "base.h"
#include "controller.h"
#include "rowpolicy.h"
#include "scheduler.h"

namespace Ramulator
{

class OpenRowPolicy : public IRowPolicy, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IRowPolicy, OpenRowPolicy, "OpenRowPolicy",
                                      "Open Row Policy.")
  private:
  public:
    void init() override {};

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override {};

    void update(bool request_found, ReqBuffer::iterator& req_it) override {
        // OpenRowPolicy does not need to take any actions
    };
};

class ClosedRowPolicy : public IRowPolicy, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IRowPolicy, ClosedRowPolicy, "ClosedRowPolicy",
                                      "Close Row Policy.")
  private:
    IDRAM* m_dram;

    int m_PRE_req_id = -1;

    int m_cap = -1;

    int m_rank_level = -1;
    int m_bankgroup_level = -1;
    int m_bank_level = -1;
    int m_row_level = -1;
    int m_num_ranks = -1;
    int m_num_bankgroups = -1;
    int m_num_banks = -1;

    int s_num_close_reqs = 0;

    std::vector<uint64_t> m_col_accesses;

  public:
    void init() override {};

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;

    void update(bool request_found, ReqBuffer::iterator& req_it) override;
};

} // namespace Ramulator
