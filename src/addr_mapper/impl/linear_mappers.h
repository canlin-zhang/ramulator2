#pragma once
#include <vector>

#include "base.h"
#include "dram.h"
#include "addr_mapper.h"
#include "memory_system.h"

namespace Ramulator
{

    class LinearMapperBase : public IAddrMapper
    {
    public:
        IDRAM *m_dram = nullptr;

        int m_num_levels = -1;        // How many levels in the hierarchy?
        std::vector<int> m_addr_bits; // How many address bits for each level in the hierarchy?
        Addr_t m_tx_offset = -1;

        int m_col_bits_idx = -1;
        int m_row_bits_idx = -1;

    protected:
        void setup(IFrontEnd *frontend, IMemorySystem *memory_system);
    };

    class ChRaBaRoCo final : public LinearMapperBase, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IAddrMapper, ChRaBaRoCo, "ChRaBaRoCo", "Applies a trival mapping to the address.");

    public:
        void init() override {};

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        void apply(Request &req) override;
    };

    class RoBaRaCoCh final : public LinearMapperBase, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IAddrMapper, RoBaRaCoCh, "RoBaRaCoCh", "Applies a RoBaRaCoCh mapping to the address.");

    public:
        void init() override {};

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        void apply(Request &req) override;
    };

    class MOP4CLXOR final : public LinearMapperBase, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IAddrMapper, MOP4CLXOR, "MOP4CLXOR", "Applies a MOP4CLXOR mapping to the address.");

    public:
        void init() override {};

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override;

        void apply(Request &req) override;
    };

} // namespace Ramulator