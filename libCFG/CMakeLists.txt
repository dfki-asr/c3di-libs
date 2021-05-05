cmake_minimum_required(VERSION 3.10)
project(libCFG)

INCLUDE(../cmake/CommonFlags)
INCLUDE(../cmake/GetSourceFiles)

# find all directories containing source files
set(FILE_TYPES *.c *.cc *.cpp *.h *.hpp *.tcc)
find_directories(DIRS src "${FILE_TYPES}")

# generate source tree
generate_source_tree(HOST_SOURCES "${DIRS}" "${FILE_TYPES}")

# this is for other tools to have access to project headers
set(LIBCFG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIBCFG_INCLUDE_DIR ${LIBCFG_INCLUDE_DIR} PARENT_SCOPE)

IF(CMAKE_COMPILER_IS_GNUCXX)
add_library(${PROJECT_NAME} SHARED ${HOST_SOURCES} )
ELSE()
add_library(${PROJECT_NAME} STATIC ${HOST_SOURCES} )
ENDIF()

# set include directories
message(${LIBMMV_INCLUDE_DIR})
target_include_directories(libCFG PUBLIC "${LIBCFG_INCLUDE_DIR}"
                    "${LIBMMV_INCLUDE_DIR}"
                    "${RAPID_XML_INCLUDE_DIR}"
		    "${GTEST_INCLUDE_DIR}"
		    "${MEMORYMANAGER_INCLUDE_DIR}")