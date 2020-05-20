cmake_minimum_required(VERSION 2.8)

project(Libraries)

# include cmake modules
set(CMAKE_MODULE_PATH      ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_BINARY_DIR       ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${EXECUTABLE_OUTPUT_PATH})

include(CommonFlags)
include(GetSourceFiles)
include(PrecompiledHeaders)
include(Cpp11)

SwitchOnCpp11()

find_package(Dependencies)

# memory manager
add_subdirectory(memoryManager)

# lib MMV
add_subdirectory(libMMV)
add_subdirectory(libMMV/test)
# add_dependencies(libMMVUnitTest libMMV)

# lib Thread Pool
add_subdirectory(libThreadPool)

# lib Thread Pool
add_subdirectory(libTimer)

# lib Program Config
add_subdirectory(libCFG)
add_dependencies(libCFG libMMV)
