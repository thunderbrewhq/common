cmake_minimum_required(VERSION 3.5)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds not allowed.
        Please make a new directory (called a build directory) and run CMake from there.
        You may need to remove CMakeCache.txt."
    )
endif()

if(TARGET common)
    # Guard for use as transitive dependency
    return()
endif()

# Project
project(common)

set(CMAKE_CXX_STANDARD 11)

include(lib/system/cmake/system.cmake)

add_subdirectory(lib)
add_subdirectory(common)
add_subdirectory(test)
add_subdirectory(vendor)
