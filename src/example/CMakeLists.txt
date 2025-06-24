set(EXAMPLE_FILES
  example_ifce.h
  impl/example_impl.cpp
  impl/example_serialization.cpp
  impl/complicated_impl.h impl/complicated_impl.cpp
  impl/another_impl.cpp
  impl/yetanother_impl.cpp
)

add_library(ramulator-example STATIC ${EXAMPLE_FILES})

target_link_libraries(ramulator-example PUBLIC
  ramulator-base
  ramulator-dram
  ramulator-controller
  ramulator-memorysystem
  ramulator-frontend
  ramulator-addrmapper
  ramulator-translation
  yaml-cpp::yaml-cpp
  spdlog::spdlog
)

target_include_directories(ramulator-example
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
