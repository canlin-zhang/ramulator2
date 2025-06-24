add_subdirectory(base)
add_subdirectory(frontend)
add_subdirectory(translation)
add_subdirectory(memory_system)
add_subdirectory(addr_mapper)
add_subdirectory(dram)
add_subdirectory(dram_controller)

add_executable(ramulator
  main.cpp
)

target_link_libraries(ramulator PRIVATE
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

target_include_directories(ramulator
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
