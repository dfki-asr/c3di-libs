cmake_minimum_required(VERSION 2.8)
project(libThreadPool)

INCLUDE(../cmake/CommonFlags)
INCLUDE(../cmake/GetSourceFiles)

# find all directories containing source files
set(FILE_TYPES *.c *.cc *.cpp *.h *.hpp *.tcc)
find_directories(DIRS src "${FILE_TYPES}")

# generate source tree
generate_source_tree(HOST_SOURCES "${DIRS}" "${FILE_TYPES}")

# this is for other tools to have access to project headers
set(LIBTP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIBTP_INCLUDE_DIR ${LIBTP_INCLUDE_DIR} PARENT_SCOPE)

# set include directories
include_directories("${LIBTP_INCLUDE_DIR}"
					"${GTEST_INCLUDE_DIR}"
					"${MEMORYMANAGER_INCLUDE_DIR}"
					)

IF(CMAKE_COMPILER_IS_GNUCXX)
add_library(${PROJECT_NAME} SHARED ${HOST_SOURCES} )
ELSE()
add_library(${PROJECT_NAME} STATIC ${HOST_SOURCES} )
ENDIF()

if(CMAKE_COMPILER_IS_GNUCC)
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} pthread)
endif(CMAKE_COMPILER_IS_GNUCC)
