#pragma once
#include "impl/no_translation.h"
#include "impl/random_translation.h"

namespace Ramulator
{
    void register_translation()
    {
        // Register translations
        (void)&NoTranslation::init;
        (void)&RandomTranslation::init;
    }
} // namespace Ramulator