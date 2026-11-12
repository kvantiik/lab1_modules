cmake_minimum_required(VERSION 4.1)
project(lab1_modules LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(modules)

add_executable(lab1
        src/main.cpp
)

target_link_libraries(lab1
        PRIVATE
        cache_modules
)

add_subdirectory(tests)
