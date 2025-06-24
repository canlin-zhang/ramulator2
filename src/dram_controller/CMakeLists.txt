set(CONTROLLER_FILES
  bh_controller.h
  bh_scheduler.h
  controller.h
  register_controller.h
  scheduler.h
  plugin.h
  refresh.h
  rowpolicy.h

  impl/bh_dram_controller.h impl/bh_dram_controller.cpp
  impl/dummy_controller.h
  impl/generic_dram_controller.h impl/generic_dram_controller.cpp
  impl/prac_dram_controller.h impl/prac_dram_controller.cpp

  impl/scheduler/bh_scheduler.h impl/scheduler/bh_scheduler.cpp
  impl/scheduler/blocking_scheduler.h impl/scheduler/blocking_scheduler.cpp
  impl/scheduler/generic_scheduler.h impl/scheduler/generic_scheduler.cpp
  impl/scheduler/bliss_scheduler.h impl/scheduler/bliss_scheduler.cpp
  impl/scheduler/prac_scheduler.h impl/scheduler/prac_scheduler.cpp

  impl/refresh/all_bank_refresh.h impl/refresh/all_bank_refresh.cpp

  impl/rowpolicy/basic_rowpolicies.h impl/rowpolicy/basic_rowpolicies.cpp

  impl/plugin/trace_recorder.hpp
  impl/plugin/cmd_counter.hpp
  impl/plugin/para.hpp
  impl/plugin/graphene.hpp
  impl/plugin/oracle_rh.hpp
  impl/plugin/twice.hpp
  impl/plugin/hydra.hpp
  impl/plugin/rrs.hpp
  impl/plugin/aqua.hpp
  impl/plugin/rfm_manager.hpp

  impl/plugin/blockhammer/blockhammer_throttler.h
  impl/plugin/blockhammer/blockhammer_util.h
  impl/plugin/blockhammer/blockhammer.cpp
  impl/plugin/blockhammer/blockhammer.h

  impl/plugin/device_config/device_config.cpp
  impl/plugin/device_config/device_config.h

  impl/plugin/bliss/bliss.cpp
  impl/plugin/bliss/bliss.h

  impl/plugin/prac/prac.cpp
  impl/plugin/prac/prac.h
)

add_library(ramulator-controller STATIC ${CONTROLLER_FILES})

target_link_libraries(ramulator-controller PUBLIC
  ramulator-base
  ramulator-dram
  ramulator-translation
)

target_include_directories(ramulator-controller
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
