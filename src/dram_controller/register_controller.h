#pragma once
#include "impl/bh_dram_controller.h"
#include "impl/dummy_controller.h"
#include "impl/generic_dram_controller.h"
#include "impl/prac_dram_controller.h"

namespace Ramulator
{
    // Register all controller
    void register_controller()
    {
        // Register DRAM controllers
        (void)&BHDRAMController::init;
        (void)&DummyController::init;
        (void)&GenericDRAMController::init;
        (void)&PRACDRAMController::init;
    }
}