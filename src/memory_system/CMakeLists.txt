set(MEMORYSYSTEM_FILES
  bh_memory_system.h
  memory_system.h

  impl/bh_DRAM_system.h impl/bh_DRAM_system.cpp
  impl/dummy_memory_system.h
  impl/generic_DRAM_system.h impl/generic_DRAM_system.cpp
)

add_library(ramulator-memorysystem STATIC ${MEMORYSYSTEM_FILES})

target_link_libraries(ramulator-memorysystem PUBLIC
  ramulator-base
  ramulator-addrmapper
  ramulator-dram
  ramulator-frontend
  ramulator-translation
)

target_include_directories(ramulator-memorysystem
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
