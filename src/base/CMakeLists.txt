set(BASE_FILES
  base.h
  factory.h factory.cpp
  type.h
  exception.h
  logging.h logging.cpp
  debug.h
  param.h
  utils.h utils.cpp
  config.h config.cpp
  clocked.h
  stats.h stats.cpp
  request.h request.cpp
  serialization.h
)

add_library(ramulator-base STATIC ${BASE_FILES})

target_link_libraries(ramulator-base PUBLIC
  ramulator-frontend
  spdlog::spdlog
  yaml-cpp::yaml-cpp
)

target_include_directories(ramulator-base
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
