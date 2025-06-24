#pragma once
// DRAM controllers
#include "impl/bh_dram_controller.h"
#include "impl/dummy_controller.h"
#include "impl/generic_dram_controller.h"
#include "impl/prac_dram_controller.h"

// Refresh policies
#include "impl/refresh/all_bank_refresh.h"

// Row policies
#include "impl/rowpolicy/basic_rowpolicies.h"

// Schdulers
#include "impl/scheduler/bh_scheduler.h"
#include "impl/scheduler/bliss_scheduler.h"
#include "impl/scheduler/blocking_scheduler.h"
#include "impl/scheduler/generic_scheduler.h"
#include "impl/scheduler/prac_scheduler.h"

// Plugins
#include "impl/plugin/aqua.hpp"
#include "impl/plugin/cmd_counter.hpp"
#include "impl/plugin/graphene.hpp"
#include "impl/plugin/hydra.hpp"
#include "impl/plugin/oracle_rh.hpp"
#include "impl/plugin/para.hpp"
#include "impl/plugin/rfm_manager.hpp"
#include "impl/plugin/rrs.hpp"
#include "impl/plugin/trace_recorder.hpp"
// #include "impl/plugin/trr.hpp"
#include "impl/plugin/twice.hpp"

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

// Register all schedulers
void register_scheduler()
{
    // Register schedulers
    (void)&BHScheduler::init;
    (void)&FRFCFS::init;
    (void)&PRACScheduler::init;
    (void)&BLISSScheduler::init;
    (void)&BlockingScheduler::init;
}

// Register all row policies
void register_row_policy()
{
    (void)&OpenRowPolicy::init;
    (void)&ClosedRowPolicy::init;
}

// Register all refresh policies
void register_refresh_policy()
{
    (void)&AllBankRefresh::init;
}

// Register all plugins
void register_plugin()
{
    (void)&AQUA::init;
    (void)&CommandCounter::init;
    (void)&Graphene::init;
    (void)&Hydra::init;
    (void)&OracleRH::init;
    (void)&PARA::init;
    (void)&RFMManager::init;
    (void)&RRS::init;
    (void)&TraceRecorder::init;
    // (void)&CounterBasedTRR::init;
    (void)&TWiCeIdeal::init;
}
} // namespace Ramulator