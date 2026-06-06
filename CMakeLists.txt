cmake_minimum_required(VERSION 3.11)
project(MyWorkflow LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

set(FISHNET_APPS OFF)
SET(FISHNET_TEST OFF)
FetchContent_Declare(
    fishnet
    GIT_REPOSITORY https://gitlab2.informatik.uni-wuerzburg.de/log66jr/fishnet
    GIT_TAG main
)

FetchContent_MakeAvailable(fishnet)


# Add src folder to the build (this will include the src/CMakeLists.txt)
add_subdirectory(src)

message(STATUS "Available CMake Targets:")
get_property(known_targets GLOBAL PROPERTY TARGETS)
foreach(target ${known_targets})
    message(STATUS "  - ${target}")
endforeach() 

add_subdirectory(workflow)