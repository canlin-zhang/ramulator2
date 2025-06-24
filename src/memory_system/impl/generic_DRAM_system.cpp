#include "generic_DRAM_system.h"

namespace Ramulator
{
  void GenericDRAMSystem::init()
  {
    // Create device (a top-level node wrapping all channel nodes)
    m_dram = create_child_ifce<IDRAM>();
    m_addr_mapper = create_child_ifce<IAddrMapper>();

    int num_channels = m_dram->get_level_size("channel");

    // Create memory controllers
    for (int i = 0; i < num_channels; i++)
    {
      IDRAMController *controller = create_child_ifce<IDRAMController>();
      controller->m_impl->set_id(fmt::format("Channel {}", i));
      controller->m_channel_id = i;
      m_controllers.push_back(controller);
    }

    m_clock_ratio = param<uint>("clock_ratio").required();

    register_stat(m_clk).name("memory_system_cycles");
    register_stat(s_num_read_requests).name("total_num_read_requests");
    register_stat(s_num_write_requests).name("total_num_write_requests");
    register_stat(s_num_other_requests).name("total_num_other_requests");
  }

  bool GenericDRAMSystem::send(Request req)
  {
    m_addr_mapper->apply(req);
    int channel_id = req.addr_vec[0];
    bool is_success = m_controllers[channel_id]->send(req);

    if (is_success)
    {
      switch (req.type_id)
      {
      case Request::Type::Read:
      {
        s_num_read_requests++;
        break;
      }
      case Request::Type::Write:
      {
        s_num_write_requests++;
        break;
      }
      default:
      {
        s_num_other_requests++;
        break;
      }
      }
    }

    return is_success;
  }

  void GenericDRAMSystem::tick()
  {
    m_clk++;
    m_dram->tick();
    for (auto controller : m_controllers)
    {
      controller->tick();
    }
  }

  float GenericDRAMSystem::get_tCK()
  {
    return m_dram->m_timing_vals("tCK_ps") / 1000.0f;
  }

} // namespace
