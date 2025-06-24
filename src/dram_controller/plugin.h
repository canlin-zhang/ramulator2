#ifndef RAMULATOR_CONTROLLER_PLUGIN_H
#define RAMULATOR_CONTROLLER_PLUGIN_H

#include <string>
#include <vector>

#include "base.h"

namespace Ramulator
{

  class IControllerPlugin
  {
    RAMULATOR_REGISTER_INTERFACE(IControllerPlugin, "ControllerPlugin", "Plugins for the memory controller.");

class IControllerPlugin
{
    RAMULATOR_REGISTER_INTERFACE(IControllerPlugin, "ControllerPlugin",
                                 "Plugins for the memory controller.");

  protected:
    IDRAMController* m_ctrl = nullptr;

  public:
    virtual void update(bool request_found, ReqBuffer::iterator& req_it) = 0;
};

} // namespace Ramulator

#endif // RAMULATOR_CONTROLLER_PLUGIN_H