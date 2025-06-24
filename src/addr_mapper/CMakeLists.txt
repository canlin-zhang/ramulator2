set(ADDRMAPPER_FILES
  addr_mapper.h
  register_addr_mapper.h

  impl/linear_mappers.h impl/linear_mappers.cpp
  impl/rit.h impl/rit.cpp
)

add_library(ramulator-addrmapper STATIC ${ADDRMAPPER_FILES})

target_link_libraries(ramulator-addrmapper PUBLIC
  ramulator-base
  ramulator-controller
  yaml-cpp::yaml-cpp
  spdlog::spdlog
)

target_include_directories(ramulator-addrmapper
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
