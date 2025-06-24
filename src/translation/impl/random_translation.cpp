#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

#include "random_translation.h"

namespace Ramulator
{
void RandomTranslation::init()
{
    int seed = param<int>("seed")
                   .desc("The seed for the random number generator used to allocate pages.")
                   .default_val(123);
    m_allocator_rng.seed(seed);

    m_max_paddr =
        param<Addr_t>("max_addr").desc("Max physical address of the memory system.").required();
    m_pagesize = param<Addr_t>("pagesize_KB").desc("Pagesize in KB.").default_val(4) << 10;
    m_offsetbits = calc_log2(m_pagesize);

    // Initially, all physical pages are free
    m_num_pages = m_max_paddr / m_pagesize;
    m_free_physical_pages.resize(m_num_pages, true);
    m_num_free_physical_pages = m_num_pages;

    m_frontend = cast_parent<IFrontEnd>();
    m_translation.resize(m_frontend->get_num_cores());

    m_logger = Logging::create_logger("RandomTranslation");
}

bool RandomTranslation::translate(Request& req)
{
    Addr_t vpn = req.addr >> m_offsetbits;

    auto& core_translation = m_translation[req.source_id];
    auto target = core_translation.find(vpn);
    if (target == core_translation.end())
    {
        // No previous translation record. Assign a new page
        if (m_num_free_physical_pages == 0)
        {
            // We run out of physical pages. Randomly replace a previously assigned page (swap
            // latency not modeled!)
            Addr_t ppn_to_replace = m_allocator_rng() % m_num_pages;
            // We do not replace a reserved page
            while (m_reserved_pages.find(ppn_to_replace) != m_reserved_pages.end())
            {
                ppn_to_replace = m_allocator_rng() % m_num_pages;
            }
            core_translation[vpn] = ppn_to_replace;
            m_logger->warn("Swapping out PPN {} for Addr {}, VPN {}.", ppn_to_replace, req.addr,
                           vpn);
        }
        else
        {
            // We have available physical pages. Randomly assign one.
            Addr_t ppn_to_assign = m_allocator_rng() % m_num_pages;
            // We do not assign a reserved page or an already assigned page
            while ((m_reserved_pages.find(ppn_to_assign) != m_reserved_pages.end()) ||
                   (!m_free_physical_pages[ppn_to_assign]))
            {
                ppn_to_assign = m_allocator_rng() % m_num_pages;
            }
            core_translation[vpn] = ppn_to_assign;
            m_num_free_physical_pages--;
        }
    }

    // We either found an existing translation or have assigned a new page
    Addr_t p_addr =
        (core_translation[vpn] << m_offsetbits) | (req.addr & ((1 << m_offsetbits) - 1));

    DEBUG_LOG(DTRANSLATE, m_logger, "Translated Addr {}, VPN {} to Addr {}, PPN {}.", req.addr, vpn,
              p_addr, core_translation[vpn]);

    req.addr = p_addr;
    return true;
}

bool RandomTranslation::reserve(const std::string& type, Addr_t addr)
{
    Addr_t ppn = addr >> m_offsetbits;
    // Add page to reserved pages if it is not already reserved
    m_reserved_pages.insert(ppn);
    // std::cout << "Reserved PPN " << ppn << "." << std::endl;
    return true;
}

Addr_t RandomTranslation::get_max_addr()
{
    return m_max_paddr;
}

} // namespace Ramulator