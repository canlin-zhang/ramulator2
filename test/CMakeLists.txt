set(TEST_FILES
  test_ifce.h
  test_impl.cpp
)

add_library(ramulator-test STATIC ${TEST_FILES})

target_link_libraries(ramulator-test PUBLIC
  ramulator-base
  yaml-cpp::yaml-cpp
  spdlog::spdlog
)

target_include_directories(ramulator-test
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
