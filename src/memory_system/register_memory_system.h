#pragma once
#include "impl/generic_DRAM_system.h"
#include "impl/bh_DRAM_system.h"
#include "impl/dummy_memory_system.h"

namespace Ramulator
{
    void register_memory_system()
    {
        // Register memory systems
        (void)&GenericDRAMSystem::init;
        (void)&BHDRAMSystem::init;
        (void)&DummyMemorySystem::init;
    }
} // namespace Ramulator