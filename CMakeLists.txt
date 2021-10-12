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

set(LIBGTEST_INCLUDE_DIR "gtest-1.10.0/include" PARENT_SCOPE)

# include cmake modules
set(CMAKE_MODULE_PATH      ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${EXECUTABLE_OUTPUT_PATH})

include(CommonFlags)
include(GetSourceFiles)

find_package(Dependencies)

# memory manager
add_subdirectory(memoryManager)
set(LIBMEMORYMANAGER_INCLUDE_DIR ${MEMORYMANAGER_INCLUDE_DIR} PARENT_SCOPE)

# lib MMV
add_subdirectory(libMMV)
set(LIBMMV_INCLUDE_DIR ${LIBMMV_INCLUDE_DIR} PARENT_SCOPE)

# lib Thread Pool
add_subdirectory(libThreadPool)
set(LIBTP_INCLUDE_DIR ${LIBTP_INCLUDE_DIR} PARENT_SCOPE)

# lib Thread Pool
add_subdirectory(libTimer)
set(LIBTIMER_INCLUDE_DIR ${LIBTIMER_INCLUDE_DIR} PARENT_SCOPE)

# lib Program Config
add_subdirectory(libCFG)
add_dependencies(libCFG libmmv)
set(LIBCFG_INCLUDE_DIR ${LIBCFG_INCLUDE_DIR} PARENT_SCOPE)
