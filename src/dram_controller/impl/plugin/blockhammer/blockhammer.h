#pragma once
#include "addr_mapper.h"
#include "base.h"
#include "controller.h"
#include "frontend.h"
#include "impl/processor/bhO3/bhO3.h"
#include "impl/processor/bhO3/bhllc.h"
#include "plugin.h"
#include "translation.h"

#include <array>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "blockhammer.h"
#include "blockhammer_throttler.h"
#include "blockhammer_util.h"

namespace Ramulator
{

class IBlockHammer
{
  public:
    virtual bool is_act_safe(Request& req) = 0;
};

class BlockHammer : public IControllerPlugin, public Implementation, public IBlockHammer
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IControllerPlugin, BlockHammer, "BlockHammer", "BlockHammer")

    typedef int elem_t;
    typedef uint16_t ctr_t;
    using SubFilter = CountingBloomFilter<elem_t, ctr_t>;
    using BaseFilter = UnifiedBloomFilter<elem_t, SubFilter>;

  public:
    // blockhammer configuration parameters
    int m_bf_num_filters = -1;
    int m_bf_len_epoch = -1;
    int m_bf_ctr_count = -1;
    int m_bf_ctr_thresh = -1;
    bool m_bf_ctr_saturate = -1;
    int m_bf_num_hashes = -1;
    int m_bf_hist_size = -1;
    int m_bf_hist_max_freq = -1;
    int m_bf_num_rh = -1;
    int m_bf_trefw = -1;
    int m_bf_trc = -1;

    int m_bf_len_epoch_clk = -1;

  private:
    IDRAM* m_dram = nullptr;
    BHO3LLC* m_llc;
    std::vector<BaseFilter*> m_filters;
    std::vector<HistoryBuffer<elem_t>*> m_histbufs;
    std::unordered_set<int> m_blacklisted_rows;
    std::vector<std::unordered_map<int, int>*> m_activations;
    AttackThrottler* m_attack_throttler;

    int m_clk = -1;

    int m_num_mshr_per_core = -1;

    // input parameters
    int m_rank_level = -1;
    int m_bank_group_level = -1;
    int m_bank_level = -1;
    int m_row_level = -1;

    int m_num_ranks = -1;
    int m_num_banks_per_rank = -1;
    int m_num_rows_per_bank = -1;

    bool m_is_debug;

    BaseFilter* get_bank_filter(Request& req);

  public:
    void init() override;

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override;

    void update(bool request_found, ReqBuffer::iterator& req_it) override;

    bool is_act_safe(Request& req);
}; // class BlockHammer

} //  namespace Ramulator