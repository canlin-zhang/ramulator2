add_subdirectory(base)
add_subdirectory(frontend)
add_subdirectory(translation)
add_subdirectory(memory_system)
add_subdirectory(addr_mapper)
add_subdirectory(dram)
add_subdirectory(dram_controller)

add_executable(ramulator2
  register_all.h
  main.cpp
)

target_link_libraries(ramulator2 PRIVATE
  ramulator-base
  ramulator-frontend
  ramulator-translation
  ramulator-memorysystem
  ramulator-addrmapper
  ramulator-dram
  ramulator-controller
  yaml-cpp::yaml-cpp
  spdlog::spdlog
  argparse::argparse
)

target_include_directories(ramulator2
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(ramulator2 PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
