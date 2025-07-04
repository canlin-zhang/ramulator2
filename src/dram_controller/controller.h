#ifndef RAMULATOR_CONTROLLER_CONTROLLER_H
#define RAMULATOR_CONTROLLER_CONTROLLER_H

#include <deque>
#include <vector>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "base.h"
#include "dram.h"
#include "plugin.h"
#include "refresh.h"
#include "rowpolicy.h"
#include "scheduler.h"

namespace Ramulator
{

class IDRAMController : public Clocked<IDRAMController>
{
    RAMULATOR_REGISTER_INTERFACE(IDRAMController, "Controller", "Memory Controller Interface");

  public:
    IDRAM* m_dram = nullptr;
    IScheduler* m_scheduler = nullptr;
    IRefreshManager* m_refresh = nullptr;
    IRowPolicy* m_rowpolicy = nullptr;
    std::vector<IControllerPlugin*> m_plugins;

    int m_channel_id = -1;

  public:
    /**
     * @brief       Send a request to the memory controller.
     *
     * @param    req        The request to be enqueued.
     * @return   true       Successful.
     * @return   false      Failed (e.g., buffer full).
     */
    virtual bool send(Request& req) = 0;

    /**
     * @brief       Send a high-priority request to the memory controller.
     *
     */
    virtual bool priority_send(Request& req) = 0;

    /**
     * @brief       Ticks the memory controller.
     *
     */
    virtual void tick() = 0;
};

} // namespace Ramulator

#endif // RAMULATOR_CONTROLLER_CONTROLLER_H