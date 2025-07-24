#ifndef RAMULATOR_BHMEMORYSYSTEM_MEMORY_H
#define RAMULATOR_BHMEMORYSYSTEM_MEMORY_H

#include "dram/dram.h"
#include "memory_system/memory_system.h"

namespace Ramulator
{

class IBHMemorySystem : public IMemorySystem
{
    RAMULATOR_REGISTER_INTERFACE(
        IBHMemorySystem, "BHMemorySystem",
        "BH Memory system interface (e.g., communicates between processor and memory controller).")
  public:
    virtual IDRAM* get_dram()
    {
        return nullptr;
    }
};

} // namespace Ramulator

#endif // RAMULATOR_BHMEMORYSYSTEM_MEMORY_H