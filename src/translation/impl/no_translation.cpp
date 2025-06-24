#include <iostream>
#include <vector>
#include <random>

#include "no_translation.h"

namespace Ramulator
{
  void NoTranslation::init()
  {
    m_max_paddr = param<Addr_t>("max_addr").desc("Max physical address of the memory system.").required();
  }

  bool NoTranslation::translate(Request &req)
  {
    // We dont do any translation. Just wrap the vaddr around max_paddr.
    // Addr_t new_addr = (req.addr % m_max_paddr);
    Addr_t new_addr = (req.addr);
    req.addr = new_addr;
    return true;
  }

} // namespace Ramulator