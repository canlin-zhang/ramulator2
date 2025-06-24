set(DRAM_FILES
  dram.h
  register_dram.h
  node.h
  spec.h
  lambdas.h

  lambdas/preq.h
  lambdas/rowhit.h
  lambdas/rowopen.h
  lambdas/action.h
  lambdas/power.h

  impl/DDR3.hpp
  impl/DDR4.hpp
  impl/DDR4-VRR.hpp
  impl/DDR4-RVRR.hpp
  impl/DDR5.hpp
  impl/DDR5-VRR.hpp
  impl/DDR5-RVRR.hpp
  impl/LPDDR5.hpp
  impl/HBM.hpp
  impl/HBM2.hpp
  impl/HBM3.hpp
)

add_library(ramulator-dram STATIC ${DRAM_FILES})

target_link_libraries(ramulator-dram PUBLIC
  ramulator-base
  yaml-cpp::yaml-cpp
  spdlog::spdlog
)

target_include_directories(ramulator-dram
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
