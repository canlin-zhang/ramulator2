#include "linear_mappers.h"

namespace Ramulator
{
  void LinearMapperBase::setup(IFrontEnd *frontend, IMemorySystem *memory_system)
  {
    m_dram = memory_system->get_ifce<IDRAM>();

    // Populate m_addr_bits vector with the number of address bits for each level in the hierachy
    const auto &count = m_dram->m_organization.count;
    m_num_levels = count.size();
    m_addr_bits.resize(m_num_levels);
    for (size_t level = 0; level < m_addr_bits.size(); level++)
    {
      m_addr_bits[level] = calc_log2(count[level]);
    }

    // Last (Column) address have the granularity of the prefetch size
    m_addr_bits[m_num_levels - 1] -= calc_log2(m_dram->m_internal_prefetch_size);

    int tx_bytes = m_dram->m_internal_prefetch_size * m_dram->m_channel_width / 8;
    m_tx_offset = calc_log2(tx_bytes);

    // Determine where are the row and col bits for ChRaBaRoCo and RoBaRaCoCh
    try
    {
      m_row_bits_idx = m_dram->m_levels("row");
    }
    catch (const std::out_of_range &r)
    {
      throw std::runtime_error(fmt::format("Organization \"row\" not found in the spec, cannot use linear mapping!"));
    }

    // Assume column is always the last level
    m_col_bits_idx = m_num_levels - 1;
  }

  void ChRaBaRoCo::setup(IFrontEnd *frontend, IMemorySystem *memory_system)
  {
    LinearMapperBase::setup(frontend, memory_system);
  }

  void ChRaBaRoCo::apply(Request &req)
  {
    req.addr_vec.resize(m_num_levels, -1);
    Addr_t addr = req.addr >> m_tx_offset;
    for (int i = m_addr_bits.size() - 1; i >= 0; i--)
    {
      req.addr_vec[i] = slice_lower_bits(addr, m_addr_bits[i]);
    }
  }

  void RoBaRaCoCh::setup(IFrontEnd *frontend, IMemorySystem *memory_system)
  {
    LinearMapperBase::setup(frontend, memory_system);
  }

  void RoBaRaCoCh::apply(Request &req)
  {
    req.addr_vec.resize(m_num_levels, -1);
    Addr_t addr = req.addr >> m_tx_offset;
    req.addr_vec[0] = slice_lower_bits(addr, m_addr_bits[0]);
    req.addr_vec[m_addr_bits.size() - 1] = slice_lower_bits(addr, m_addr_bits[m_addr_bits.size() - 1]);
    for (int i = 1; i <= m_row_bits_idx; i++)
    {
      req.addr_vec[i] = slice_lower_bits(addr, m_addr_bits[i]);
    }
  }

  void MOP4CLXOR::setup(IFrontEnd *frontend, IMemorySystem *memory_system)
  {
    LinearMapperBase::setup(frontend, memory_system);
  }

  void MOP4CLXOR::apply(Request &req)
  {
    req.addr_vec.resize(m_num_levels, -1);
    Addr_t addr = req.addr >> m_tx_offset;
    req.addr_vec[m_col_bits_idx] = slice_lower_bits(addr, 2);
    for (int lvl = 0; lvl < m_row_bits_idx; lvl++)
      req.addr_vec[lvl] = slice_lower_bits(addr, m_addr_bits[lvl]);
    req.addr_vec[m_col_bits_idx] += slice_lower_bits(addr, m_addr_bits[m_col_bits_idx] - 2) << 2;
    req.addr_vec[m_row_bits_idx] = (int)addr;

    int row_xor_index = 0;
    for (int lvl = 0; lvl < m_col_bits_idx; lvl++)
    {
      if (m_addr_bits[lvl] > 0)
      {
        int mask = (req.addr_vec[m_col_bits_idx] >> row_xor_index) & ((1 << m_addr_bits[lvl]) - 1);
        req.addr_vec[lvl] = req.addr_vec[lvl] xor mask;
        row_xor_index += m_addr_bits[lvl];
      }
    }
  }

}