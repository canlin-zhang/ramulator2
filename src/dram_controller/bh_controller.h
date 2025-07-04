#ifndef RAMULATOR_CONTROLLER_LIVECONTROLLER_H
#define RAMULATOR_CONTROLLER_LIVECONTROLLER_H

#include <deque>
#include <limits>
#include <vector>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "base.h"
#include "bh_scheduler.h"
#include "controller.h"
#include "dram.h"
#include "plugin.h"
#include "refresh.h"

namespace Ramulator
{

class IBHDRAMController : public IDRAMController
{
    RAMULATOR_REGISTER_INTERFACE(IBHDRAMController, "BHDRAMController",
                                 "BHammer Memory Controller Interface");

  public:
    IBHScheduler* m_scheduler = nullptr;
    virtual void tick() = 0;

    template <class T> T* get_plugin()
    {
        for (auto plugin : m_plugins)
        {
            T* cast = dynamic_cast<T*>(plugin);
            if (cast)
            {
                return cast;
            }
        }
        return nullptr;
    }

  protected:
    std::vector<IControllerPlugin*> m_plugins;
};

} // namespace Ramulator

#endif // RAMULATOR_CONTROLLER_LIVECONTROLLER_H