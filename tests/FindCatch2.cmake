include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v${Catch2_FIND_VERSION}
    GIT_SHALLOW ON
    SYSTEM)
FetchContent_MakeAvailable(Catch2)
set_target_properties(Catch2 PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
set_target_properties(Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
