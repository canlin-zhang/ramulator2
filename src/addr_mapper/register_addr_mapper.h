#pragma once
#include "impl/linear_mappers.h"
#include "impl/rit.h"

namespace Ramulator
{
void register_addr_mapper()
{
    // Register address mappers
    (void)&ChRaBaRoCo::init;
    (void)&RoBaRaCoCh::init;
    (void)&MOP4CLXOR::init;
    //(void)&LinearMapperBase_with_rit::init(...);
}
} // namespace Ramulator