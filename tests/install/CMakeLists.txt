cmake_minimum_required(VERSION 3.25)
project(LaplaceTest CXX)

if(PROJECT_IS_TOP_LEVEL)
    find_package(Laplace 0.0.1 EXACT REQUIRED)
endif()

add_executable(test-install Install.cpp)
target_link_libraries(test-install PRIVATE Laplace::Laplace)
