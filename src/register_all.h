#pragma once
#include "frontend/register_frontend.h"
#include "translation/register_translation.h"
#include "memory_system/register_memory_system.h"
#include "dram/register_dram.h"
#include "addr_mapper/register_addr_mapper.h"
#include "dram_controller/register_controller.h"

namespace Ramulator
{
    void register_all()
    {
        // Register all frontends
        register_frontend();

        // Register all memory systems
        register_memory_system();

        // Register all translations
        register_translation();

        // Register all DRAM implementations
        register_dram();

        // Register all address mappers
        register_addr_mapper();

        // Register all controllers
        register_controller();

        // Register all schedulers
        register_scheduler();

        // Register all row policies
        register_row_policy();

        // Register all refresh policies
        register_refresh_policy();

        // Register all plugins
        register_plugin();
    }

}