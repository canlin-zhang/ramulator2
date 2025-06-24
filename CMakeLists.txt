cmake_minimum_required(VERSION 3.14)

project(
  Ramulator
  VERSION 2.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# #### Prompt the build type ####
# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE "" CACHE STRING "" FORCE)
# endif()Release
# message("Configuring ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_Version}...")
# message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRAMULATOR_DEBUG")
# # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# ###############################

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Include external libraries
include(external.cmake)

add_subdirectory(src)
add_subdirectory(test)
