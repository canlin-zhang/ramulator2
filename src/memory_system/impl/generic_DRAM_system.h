#pragma once
#include "memory_system.h"
#include "translation.h"
#include "controller.h"
#include "addr_mapper.h"
#include "dram.h"

namespace Ramulator
{

    class GenericDRAMSystem final : public IMemorySystem, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(IMemorySystem, GenericDRAMSystem, "GenericDRAM", "A generic DRAM-based memory system.");

    protected:
        Clk_t m_clk = 0;
        IDRAM *m_dram;
        IAddrMapper *m_addr_mapper;
        std::vector<IDRAMController *> m_controllers;

    public:
        int s_num_read_requests = 0;
        int s_num_write_requests = 0;
        int s_num_other_requests = 0;

    public:
        void init() override;

        void setup(IFrontEnd *frontend, IMemorySystem *memory_system) override {}

        bool send(Request req) override;

        void tick() override;

        float get_tCK() override;

        // const SpecDef& get_supported_requests() override {
        //   return m_dram->m_requests;
        // };
    };

} // namespace
