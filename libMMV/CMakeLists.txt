cmake_minimum_required(VERSION 2.8)
project(libMMV)

SwitchOnCpp11()
add_definitions(/D_USE_MATH_DEFINES)

# find all directories containing source files
set(FILE_TYPES *.c *.cc *.cpp *.h *.hpp *.tcc)
find_directories(DIRS src "${FILE_TYPES}")
generate_source_tree(HOST_SOURCES "${DIRS}" "${FILE_TYPES}")

# this is for other tools to have access to libReconstruction headers
set(LIBMMV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIBMMV_INCLUDE_DIR ${LIBMMV_INCLUDE_DIR} PARENT_SCOPE)

# set include directories
include_directories("${MEMORYMANAGER_INCLUDE_DIR}"
                    "${GTEST_INCLUDE_DIR}"
                    "${FREEIMAGE_INCLUDE_DIR}"
                    "${LIBMMV_INCLUDE_DIR}"													
					)

add_library(${PROJECT_NAME} STATIC ${HOST_SOURCES} )

target_link_libraries(${PROJECT_NAME}
                      MemoryManager
                      debug
                      "${FREEIMAGE_LIBRARIES_DEBUG}"
                      optimized
                      "${FREEIMAGE_LIBRARIES_RELEASE}"
                      ${GTEST_BOTH_LIBRARIES}
                      )

# precompiled headers
add_precompiled_header(${PROJECT_NAME} stdafx.h)
