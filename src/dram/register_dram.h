#pragma once
#include "impl/DDR3.hpp"
#include "impl/DDR4.hpp"
#include "impl/DDR4-RVRR.hpp"
#include "impl/DDR4-VRR.hpp"
#include "impl/DDR5.hpp"
#include "impl/DDR5-RVRR.hpp"
#include "impl/DDR5-VRR.hpp"
#include "impl/GDDR6.hpp"
#include "impl/LPDDR5.hpp"
#include "impl/HBM.hpp"
#include "impl/HBM2.hpp"
#include "impl/HBM3.hpp"

namespace Ramulator
{
    void register_dram()
    {
        // Register DRAM implementations
        (void)&DDR3::init;
        (void)&DDR4::init;
        (void)&DDR4RVRR::init;
        (void)&DDR4VRR::init;
        (void)&DDR5::init;
        (void)&DDR5RVRR::init;
        (void)&DDR5VRR::init;
        (void)&GDDR6::init;
        (void)&LPDDR5::init;
        (void)&HBM::init;
        (void)&HBM2::init;
        (void)&HBM3::init;
    }
} // namespace Ramulator