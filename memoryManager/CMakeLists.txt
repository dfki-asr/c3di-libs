cmake_minimum_required(VERSION 3.10)

project(MemoryManager)

INCLUDE(CommonFlags)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP -Zm200")
	add_definitions(/DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_USE_MATH_DEFINES /DCOMPILING_DLL)
endif(MSVC)

# find all directories containing source files
set(FILE_TYPES *.c *.cc *.cpp *.h *.hpp *.tcc)
find_directories(DIRS src "${FILE_TYPES}")
generate_source_tree(HOST_SOURCES "${DIRS}" "${FILE_TYPES}")

# this is for other tools to have access to memoryManager headers
set(MEMORYMANAGER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(MEMORYMANAGER_INCLUDE_DIR ${MEMORYMANAGER_INCLUDE_DIR} PARENT_SCOPE)


add_library(${PROJECT_NAME} SHARED ${HOST_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${MEMORYMANAGER_INCLUDE_DIR})