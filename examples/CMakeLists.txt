include(FetchContent)

# Disable installation of 3rd party libraries
macro(install)
endmacro()

# Get 3rd party libraries
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.7)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

FetchContent_Declare(ImPlot
    GIT_REPOSITORY https://github.com/epezent/implot
    GIT_TAG v0.15)
FetchContent_MakeAvailable(ImPlot)
FetchContent_GetProperties(ImPlot SOURCE_DIR IMPLOT_DIR)

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_WARNINGS_AS_ERRORS OFF)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG bc86e0bcedd2e3d0d6b9a64f24f40008dda200e4)
FetchContent_MakeAvailable(SFML)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 3dc92900ff08e5267081fc79a2de1dc274171817)
FetchContent_MakeAvailable(ImGui-SFML)

# Add ImPlot sources to ImGui-SFML
target_sources(ImGui-SFML PRIVATE
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)
target_include_directories(ImGui-SFML PUBLIC $<BUILD_INTERFACE:${IMPLOT_DIR}>)

# Ensure tools ignore ImGui-SFML sources
get_target_property(IMGUI_SFML_INCLUDE_DIRS ImGui-SFML INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(ImGui-SFML SYSTEM INTERFACE ${IMGUI_SFML_INCLUDE_DIRS})
target_compile_definitions(ImGui-SFML PUBLIC "ImDrawIdx=unsigned int")

# Add examples
add_compile_options(${LAPLACE_WARNINGS})

add_library(view-filter ViewFilter.cpp)
target_link_libraries(view-filter PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML PUBLIC Laplace::Laplace)

add_executable(low-pass-example LowPass.cpp)
target_link_libraries(low-pass-example PRIVATE view-filter)
add_custom_target(run-low-pass-example COMMAND low-pass-example)

add_executable(high-pass-example HighPass.cpp)
target_link_libraries(high-pass-example PRIVATE view-filter)
add_custom_target(run-high-pass-example COMMAND high-pass-example)

add_custom_target(run-examples)
add_dependencies(run-examples run-low-pass-example run-high-pass-example)
