#pragma once
#include "addr_mapper.h"
#include "bh_controller.h"
#include "bh_memory_system.h"
#include "dram.h"
#include "translation.h"

namespace Ramulator
{

class BHDRAMSystem final : public IBHMemorySystem, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IMemorySystem, BHDRAMSystem, "BHDRAMSystem",
                                      "BHammer DRAM-based memory system.");

  protected:
    Clk_t m_clk = 0;
    IDRAM* m_dram;
    IAddrMapper* m_addr_mapper;
    std::vector<IBHDRAMController*> m_controllers;

  public:
    int s_num_read_requests = 0;
    int s_num_write_requests = 0;
    int s_num_other_requests = 0;

  public:
    void init() override;

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override
    {
    }

    bool send(Request req) override;

    void tick() override;

    float get_tCK() override;

    IDRAM* get_dram() override;
};

} // namespace Ramulator
