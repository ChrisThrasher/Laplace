cmake_minimum_required(VERSION 3.24)
project(Laplace VERSION 0.0.1 LANGUAGES CXX DESCRIPTION "Linear filters library")

# Add compiler warnings
option(LAPLACE_ENABLE_WARNINGS "Enable compiler warnings" OFF)
if(LAPLACE_ENABLE_WARNINGS)
    if(MSVC)
        set(LAPLACE_WARNINGS /WX /W4 /permissive- /wd4251)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(LAPLACE_WARNINGS -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
    endif()
endif()

# Add library
include(GenerateExportHeader)
add_library(Laplace src/Rng.cpp)
add_library(Laplace::Laplace ALIAS Laplace)
target_sources(Laplace PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include ${PROJECT_BINARY_DIR}/include
    FILES
        include/Laplace/Angle.hpp
        include/Laplace/Fft.hpp
        include/Laplace/Filter.hpp
        include/Laplace/Frequency.hpp
        include/Laplace/HighPass.hpp
        include/Laplace/LowPass.hpp
        include/Laplace/Rng.hpp
        ${PROJECT_BINARY_DIR}/include/Laplace/Export.hpp
)
target_include_directories(Laplace SYSTEM PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)
target_compile_features(Laplace PUBLIC cxx_std_20)
target_compile_options(Laplace PRIVATE ${LAPLACE_WARNINGS})
set_target_properties(Laplace PROPERTIES
    VERSION ${PROJECT_VERSION}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES)
generate_export_header(Laplace EXPORT_FILE_NAME include/Laplace/Export.hpp)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(Laplace PUBLIC LAPLACE_STATIC_DEFINE)
endif()

# Install library
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
install(TARGETS Laplace EXPORT LaplaceTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/Laplace-${PROJECT_VERSION}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/Laplace-${PROJECT_VERSION}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/Laplace-${PROJECT_VERSION}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Laplace-${PROJECT_VERSION})
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
option(LAPLACE_BUILD_EXAMPLES "Build examples" OFF)
if(LAPLACE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Add tests
option(LAPLACE_BUILD_TESTING "Build tests" OFF)
if(LAPLACE_BUILD_TESTING)
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
