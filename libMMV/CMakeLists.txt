cmake_minimum_required(VERSION 3.10)

set(LIBMMV_LIBRARY_NAME libmmv)
project(${LIBMMV_LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GetSourceFiles)
# find all directories containing source files
set(FILE_TYPES *.c *.cpp *.h *.hpp)
find_directories(INCLUDE_DIRS include "${FILE_TYPES}")
find_directories(SRC_DIRS src "${FILE_TYPES}")
set(DIRS ${SRC_DIRS} ${INCLUDE_DIRS})
generate_source_tree(HOST_SOURCES "${DIRS}" "${FILE_TYPES}")

if(MSVC)
	# export all symbols for a shared library
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
	add_definitions(/D_USE_MATH_DEFINES) # // Define _USE_MATH_DEFINES before including <math.h> to expose these macro # // definitions for common math constants. 
	add_definitions(/DNOMINMAX) # use Min() Max()

endif(MSVC)
add_library(${LIBMMV_LIBRARY_NAME} SHARED ${HOST_SOURCES})

# include dependencis and link libraries
set(ETTENTION_DEPENDENCIES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
find_package(FreeImage REQUIRED)
set(LIBMMV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(LIBMMV_INCLUDE_DIR ${LIBMMV_INCLUDE_DIR} PARENT_SCOPE)

target_include_directories(${LIBMMV_LIBRARY_NAME} PUBLIC "${LIBMMV_INCLUDE_DIR}" "${FREEIMAGEPLUS_INCLUDE_PATH}")
target_link_libraries(${LIBMMV_LIBRARY_NAME} PUBLIC "${FREEIMAGEPLUS_LIBRARIES}")

add_subdirectory(test)