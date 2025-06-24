#pragma once
#include "impl/processor/bhO3/bhO3.h"
#include "impl/processor/simpleO3/simpleO3.h"

namespace Ramulator
{
    void register_frontend()
    {
        // Register processors
        (void)&SimpleO3::init;
        (void)&BHO3::init;
    }
}