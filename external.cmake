include(FetchContent)
if(NOT TARGET yaml-cpp)
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(yaml-cpp)
endif()

if(NOT TARGET spdlog)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(spdlog)
endif()

if(NOT TARGET argparse)
    FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG v2.9
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(argparse)
endif()
