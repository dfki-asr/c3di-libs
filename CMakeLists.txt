cmake_minimum_required(VERSION 3.10)

project(Libraries)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# gtest
set(GOOGLETEST_VERSION 1.10.0) 
message("Configuring GoogleTest v${GOOGLETEST_VERSION}")
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
add_subdirectory("gtest-1.10.0")
set(GTEST_INCLUDE_DIR "gtest-1.10.0/include")
enable_testing()

# include cmake modules
set(CMAKE_MODULE_PATH      ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${EXECUTABLE_OUTPUT_PATH})

include(CommonFlags)
include(GetSourceFiles)

find_package(Dependencies)

# memory manager
add_subdirectory(memoryManager)

# lib MMV
add_subdirectory(libMMV)

# lib Thread Pool
add_subdirectory(libThreadPool)

# lib Thread Pool
add_subdirectory(libTimer)

# lib Program Config
add_subdirectory(libCFG)
add_dependencies(libCFG libmmv)
