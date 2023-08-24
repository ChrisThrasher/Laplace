cmake_minimum_required(VERSION 3.22)
project(Laplace VERSION 0.0.0 LANGUAGES CXX DESCRIPTION "Linear filters library")

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)

# Apply compiler warnings
option(LAPLACE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
if(NOT PROJECT_IS_TOP_LEVEL)
    set(LAPLACE_WARNINGS "")
elseif(MSVC)
    set(LAPLACE_WARNINGS $<$<BOOL:${LAPLACE_WARNINGS_AS_ERRORS}>:/WX> /W4 /permissive- /wd4251)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    set(LAPLACE_WARNINGS $<$<BOOL:${LAPLACE_WARNINGS_AS_ERRORS}>:-Werror> -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
endif()

# Add library
add_library(Laplace INTERFACE)
add_library(Laplace::Laplace ALIAS Laplace)
target_include_directories(Laplace INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
target_compile_features(Laplace INTERFACE cxx_std_17)
# target_compile_options(Laplace PRIVATE ${LAPLACE_WARNINGS})
# set_target_properties(Laplace PROPERTIES
#     VERSION ${PROJECT_VERSION}
#     CXX_VISIBILITY_PRESET hidden
#     VISIBILITY_INLINES_HIDDEN YES)
# generate_export_header(Laplace EXPORT_FILE_NAME include/Laplace/Export.hpp)
# if(NOT BUILD_SHARED_LIBS)
#     target_compile_definitions(Laplace PUBLIC LAPLACE_STATIC_DEFINE)
# endif()

# Install library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ # ${PROJECT_BINARY_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Laplace-${PROJECT_VERSION})
install(TARGETS Laplace EXPORT LaplaceTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/Laplace-${PROJECT_VERSION}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/Laplace-${PROJECT_VERSION}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/Laplace-${PROJECT_VERSION}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Laplace-${PROJECT_VERSION})
install(EXPORT LaplaceTargets
        NAMESPACE Laplace::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/Laplace-${PROJECT_VERSION}
        FILE LaplaceConfig.cmake)
write_basic_package_version_file(LaplaceConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES ${PROJECT_BINARY_DIR}/LaplaceConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/Laplace-${PROJECT_VERSION})

# Stop configuration if being consumed by a higher level project
if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

# Always build dependencies as static libraries
set(BUILD_SHARED_LIBS OFF)

# Add examples
option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Add tests
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Add development targets
add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_custom_target(tidy
    COMMAND run-clang-tidy -quiet -p ${CMAKE_BINARY_DIR} `git ls-files *.cpp`
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
