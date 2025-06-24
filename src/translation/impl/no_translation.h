#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "base.h"
#include "translation.h"
#include "frontend.h"

namespace Ramulator
{

    class NoTranslation : public ITranslation, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(ITranslation, NoTranslation, "NoTranslation", "Use the virtual address as physical address.");

    private:
        Addr_t m_max_paddr; // Max physical address

    public:
        void init() override;

        bool translate(Request &req) override;
    };

} // namespace Ramulator