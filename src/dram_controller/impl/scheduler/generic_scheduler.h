#pragma once
#include <vector>

#include "base.h"
#include "controller.h"
#include "scheduler.h"

namespace Ramulator
{

class FRFCFS : public IScheduler, public Implementation
{
    RAMULATOR_REGISTER_IMPLEMENTATION(IScheduler, FRFCFS, "FRFCFS", "FRFCFS DRAM Scheduler.")
  private:
    IDRAM* m_dram;

  public:
    void init() override {};

    void setup(IFrontEnd* frontend, IMemorySystem* memory_system) override
    {
        m_dram = cast_parent<IDRAMController>()->m_dram;
    };

    ReqBuffer::iterator compare(ReqBuffer::iterator req1, ReqBuffer::iterator req2) override;

    ReqBuffer::iterator get_best_request(ReqBuffer& buffer) override;
};

} // namespace Ramulator
