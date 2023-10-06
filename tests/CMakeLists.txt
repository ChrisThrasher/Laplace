# Get Catch2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Catch2 3.4.0 REQUIRED)
include(Catch)

option(LAPLACE_RUN_CONSTEXPR_TESTS "Run constexpr tests at runtime" OFF)

add_compile_options(${LAPLACE_WARNINGS})

# Test library
add_executable(test-laplace
    Angle.cpp
    Fft.cpp
    Filter.cpp
    Frequency.cpp
    HighPass.cpp
    LowPass.cpp
    Rng.cpp
)
target_link_libraries(test-laplace PRIVATE Laplace::Laplace Catch2::Catch2WithMain)
if(LAPLACE_RUN_CONSTEXPR_TESTS)
    target_compile_definitions(test-laplace PRIVATE CATCH_CONFIG_RUNTIME_STATIC_REQUIRE)
endif()
catch_discover_tests(test-laplace)

# Test header compilation
add_test(
    NAME "Individual header compilation"
    COMMAND ${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR} --target all_verify_interface_header_sets)

# Test install interface
add_test(NAME "Install Laplace" COMMAND
    ${CMAKE_COMMAND}
    --install ${PROJECT_BINARY_DIR}
    --prefix ${CMAKE_CURRENT_BINARY_DIR}/install/Laplace
    --config $<CONFIG>)
set_tests_properties("Install Laplace" PROPERTIES FIXTURES_SETUP install-laplace)

add_test(NAME "Configure installation test" COMMAND
    ${CMAKE_COMMAND}
    -S ${CMAKE_CURRENT_SOURCE_DIR}/install
    -B ${CMAKE_CURRENT_BINARY_DIR}/install/build
    -G ${CMAKE_GENERATOR}
    -DCMAKE_BUILD_TYPE=$<CONFIG>
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DLaplace_ROOT=${CMAKE_CURRENT_BINARY_DIR}/install/Laplace)
set_tests_properties("Configure installation test" PROPERTIES FIXTURES_SETUP configure-test-project FIXTURES_REQUIRED install-laplace)

add_test(NAME "Build installation test" COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/install/build --config $<CONFIG>)
set_tests_properties("Build installation test" PROPERTIES FIXTURES_REQUIRED configure-test-project)

add_test(NAME "Delete installation" COMMAND ${CMAKE_COMMAND} -E rm -r ${CMAKE_CURRENT_BINARY_DIR}/install/Laplace ${CMAKE_CURRENT_BINARY_DIR}/install/build)
set_tests_properties("Delete installation" PROPERTIES FIXTURES_CLEANUP "install-laplace;configure-test-project")

# Build install test project
add_subdirectory(install)
