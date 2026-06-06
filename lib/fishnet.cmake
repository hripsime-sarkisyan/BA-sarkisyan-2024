include(FetchContent)
set(FISHNET_APPS ON)
SET(FISHNET_TEST OFF)
FetchContent_Declare(fishnet
    GIT_REPOSITORY https://gitlab2.informatik.uni-wuerzburg.de/log66jr/fishnet
    GIT_TAG main
)
FetchContent_MakeAvailable(fishnet)