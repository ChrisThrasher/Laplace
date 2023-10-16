# Laplace

[![CI](https://github.com/ChrisThrasher/Laplace/actions/workflows/ci.yml/badge.svg)](https://github.com/ChrisThrasher/Laplace/actions/workflows/ci.yml)
[![Code Coverage](https://codecov.io/github/ChrisThrasher/Laplace/branch/main/graph/badge.svg?token=t85cTyvsez)](https://codecov.io/github/ChrisThrasher/Laplace)

Laplace is linear filters library written in C++20.

This library focuses on feedback controls and signal processing of physical systems where the sample rates and the dynamics of the system are typically below 1kHz.

The library's name is an homage to Pierre-Simon Laplace, the mathemetician famous for the [Laplace Transform](https://en.wikipedia.org/wiki/Laplace_transform) which is used to transform time domain functions into frequency domain functions.

## Filters

Laplace implements the following filters:

* Low Pass
* High Pass

## Compiler Support

Currently the set of tested compilers includes MSVC 19, Clang 14, AppleClang 15, and GCC 11.4.

Laplace uses a modest subset of C++20 features that have widespread support over commonly used compilers.
The library is continuously tested with the above compilers to ensure compatibility.

## Installation

You can install Laplace from source with two simple CMake commands.

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target install
```

The second command may require elevated privileges if installing to a system-wide location.
You can avoid that by installing to a local directory.

## Usage

There are two main ways to depend on Laplace

1. After installing Laplace you can use CMake's `find_package` command to depend on the library.

    ```cmake
    find_package(Laplace REQUIRED)
    ```

    CMake will automatically find Laplace if you installed it to a system-wide location.
    If you instead installed Laplace to some alternative directory you can use `-DLaplace_ROOT=<path/to/Laplace/installation>`
    during configuration of your own project to tell CMake where to find it.

1.  `FetchContent` or Git Submodules

    Laplace works just as well when consumed via `add_subdirectory` as is the case when using `FetchContent` Git submodules.

    ```cmake
    include(FetchContent)
    FetchContent_Declare(Laplace
        GIT_REPOSITORY https://github.com/ChrisThrasher/Laplace
        GIT_TAG main)
    FetchContent_MakeAvailable(Laplace)
    ```

    ```
    add_subdirectory(external/Laplace)
    ```

No matter what option you pick for retrieving Laplace linking it into your program works the same.
Simply use the `Laplace::Laplace` CMake target.

```
add_executable(app main.cpp)
target_link_libraries(app PRIVATE Laplace::Laplace)
```

## Development

Laplace is still in an early stage of development and is not ready for use.

Near term development goals:

 * Use fast Fourier transform algorithm
 * Use `lp::fft` to test frequency response of filters
 * Add signal synthesizer that uses same return type as `lp::fft`
 * Determine how to support arrays of raw values as input to `lp::fft`
 * Add Doxygen docs and docs website (only when API starts to stabilize)
 * Use custom matchers for fuzzy comparisons of aggregate types
