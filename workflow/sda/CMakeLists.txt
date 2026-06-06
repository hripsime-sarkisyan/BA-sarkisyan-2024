include(FetchContent)
FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(cli11)
include(FetchContent)
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)
set(JSON_BuildTests OFF CACHE INTERNAL "")
include(FetchContent)
FetchContent_Declare(magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum
    GIT_TAG v0.9.6
)
FetchContent_MakeAvailable(magic_enum)

set(WORKFLOW_NAME "SettlementDelineation")
set(SDA_WORKFLOW_LIB SDA_Workflow_Lib)
add_subdirectory(shared)
add_subdirectory(4_contract)
