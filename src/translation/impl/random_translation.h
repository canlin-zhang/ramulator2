#pragma once
#include <iostream>
#include <unordered_set>
#include <vector>
#include <random>

#include "base.h"
#include "translation.h"
#include "frontend.h"

namespace Ramulator
{

    class RandomTranslation : public ITranslation, public Implementation
    {
        RAMULATOR_REGISTER_IMPLEMENTATION(ITranslation, RandomTranslation, "RandomTranslation", "Randomly allocate physical pages to virtual pages.");

        IFrontEnd *m_frontend;

    protected:
        std::mt19937_64 m_allocator_rng;

        Addr_t m_max_paddr; // Max physical address
        Addr_t m_pagesize;  // Page size in bytes
        int m_offsetbits;   // The number of bits for the page offset
        size_t m_num_pages; // The total number of physical pages

        std::vector<bool> m_free_physical_pages; // The set of remaining pages.
        size_t m_num_free_physical_pages;

        using Translation_t = std::vector<std::unordered_map<Addr_t, Addr_t>>;
        Translation_t m_translation; // A vector of <vpn:ppn> maps, each core has its own map

        std::unordered_set<Addr_t> m_reserved_pages; // A vector of reserved pages

    public:
        void init() override;

        bool translate(Request &req) override;

        bool reserve(const std::string &type, Addr_t addr) override;

        Addr_t get_max_addr() override;
    };

} // namespace Ramulator