set(
  FRONTEND_FILES
  frontend.h
  register_frontend.h

  impl/memory_trace/loadstore_trace.cpp
  impl/memory_trace/readwrite_trace.cpp

  impl/processor/simpleO3/simpleO3.h impl/processor/simpleO3/simpleO3.cpp
  impl/processor/simpleO3/core.h impl/processor/simpleO3/core.cpp
  impl/processor/simpleO3/llc.h impl/processor/simpleO3/llc.cpp
  impl/processor/simpleO3/trace.h impl/processor/simpleO3/trace.cpp

  impl/processor/bhO3/bhO3.h impl/processor/bhO3/bhO3.cpp
  impl/processor/bhO3/bhcore.h impl/processor/bhO3/bhcore.cpp
  impl/processor/bhO3/bhllc.h impl/processor/bhO3/bhllc.cpp

  impl/external_wrapper/gem5_frontend.cpp
)

add_library(ramulator-frontend STATIC ${FRONTEND_FILES})

target_link_libraries(ramulator-frontend PUBLIC
  ramulator-base
  ramulator-memorysystem
)

target_include_directories(ramulator-frontend
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
