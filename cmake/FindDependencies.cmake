# Module for locating Dependencies.
#
# Customizable variables:
#   DEPENDENCIES_ROOT
#
# Read-only variables:
#   CLAMDFFT_FOUND
#     Indicates library clAmdFft has been found
#   CLAMDFFT_INCLUDE_DIR
#     Include directory for clAmdFft library
#   CLAMDFFT_LIBRARIES
#     Libraries that should be passed to target_link_libraries
#

include(FindPackageHandleStandardArgs)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#	message(STATUS "CMake thinks this is 64-bit environment")
	set(POSSIBLE_LIB_SUFFIXES Win64 x64 x86_64 lib/Win64 lib/x86_64 lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
#	message(STATUS "CMake thinks this is 32-bit environment")
	set(POSSIBLE_LIB_SUFFIXES Win32 x86 lib/Win32 lib/x86)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

find_path(DEPENDENCIES_ROOT
  NAMES "dependencties_package.txt"
  HINTS "c3di_libs_dependencies" "../c3di_libs_dependencies" "../../c3di_libs_dependencies"
  DOC "dependencies root directory")
  message(STATUS "dependencies root: ${DEPENDENCIES_ROOT}")

################
# find FreeImage
################

find_path(FREEIMAGE_ROOT_DIR
  NAMES FreeImage.h
  HINTS ${DEPENDENCIES_ROOT}
  PATH_SUFFIXES FreeImage
  DOC "FreeImage root directory")

find_path(FREEIMAGE_INCLUDE_DIR
  NAMES FreeImage.h
  HINTS ${FREEIMAGE_ROOT_DIR}
  PATH_SUFFIXES include
  DOC "FreeImage include directory")

find_library(FREEIMAGE_LIBRARY_RELEASE
  NAMES FreeImage
  HINTS ${FREEIMAGE_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})
find_library(FREEIMAGE_PLUS_LIBRARY_RELEASE
  NAMES FreeImagePlus
  HINTS ${FREEIMAGE_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

find_library(FREEIMAGE_LIBRARY_DEBUG
  NAMES FreeImaged
  HINTS ${FREEIMAGE_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})
find_library(FREEIMAGE_PLUS_LIBRARY_DEBUG
  NAMES FreeImagePlusd
  HINTS ${FREEIMAGE_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

# get path to libraries
get_filename_component(FREEIMAGE_DEBUG_DIR ${FREEIMAGE_LIBRARY_DEBUG} ABSOLUTE)
get_filename_component(FREEIMAGE_DEBUG_DIR ${FREEIMAGE_DEBUG_DIR} PATH)

get_filename_component(FREEIMAGE_RELEASE_DIR ${FREEIMAGE_LIBRARY_RELEASE} ABSOLUTE)
get_filename_component(FREEIMAGE_RELEASE_DIR ${FREEIMAGE_RELEASE_DIR} PATH)

# set important library variables
set(FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_INCLUDE_DIR} CACHE INTERNAL "FreeImage Include Directory")
set(FREEIMAGE_LIBRARIES_DEBUG ${FREEIMAGE_LIBRARY_DEBUG} ${FREEIMAGE_PLUS_LIBRARY_DEBUG} CACHE INTERNAL "FreeImage Libraries Debug")
set(FREEIMAGE_LIBRARIES_RELEASE ${FREEIMAGE_LIBRARY_RELEASE} ${FREEIMAGE_PLUS_LIBRARY_RELEASE} CACHE INTERNAL "FreeImage Libraries Release")

mark_as_advanced(FREEIMAGE_ROOT_DIR FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARIES)

find_package_handle_standard_args(FreeImage REQUIRED_VARS FREEIMAGE_ROOT_DIR
  FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARIES_DEBUG FREEIMAGE_LIBRARIES_RELEASE)


################
# find Ggtest
################

find_path(GTEST_ROOT_DIR
  NAMES lib/x64/gtest.lib
  HINTS ${DEPENDENCIES_ROOT}
  PATH_SUFFIXES gtest-1.10.0
  DOC "google test root directory")

find_path(GTEST_INCLUDE_DIR	
  NAMES gtest
  HINTS ${DEPENDENCIES_ROOT}
  PATH_SUFFIXES gtest-1.10.0/include)

find_library(GTEST_LIBRARY_RELEASE
  NAMES gtest
  HINTS ${GTEST_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

find_library(GTEST_LIBRARY_DEBUG
  NAMES gtestd
  HINTS ${GTEST_ROOT_DIR}
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})  
  
if(GTEST_INCLUDE_DIR)
  message(STATUS "  located google test in ${GTEST_INCLUDE_DIR}")
else()
  message(SEND_ERROR "google test was not found in ettention dependencies")
endif()

set(GTEST_BOTH_LIBRARIES debug ${GTEST_LIBRARY_DEBUG} optimized ${GTEST_LIBRARY_RELEASE})
find_package_handle_standard_args(gtest REQUIRED_VARS GTEST_INCLUDE_DIR GTEST_LIBRARY_DEBUG GTEST_LIBRARY_RELEASE)

################
# find Armadillo
################

find_path(ARMADILLO_ROOT_DIR
  NAMES armadillo_bits/arma_cmath.hpp
  HINTS ${DEPENDENCIES_ROOT}/armadillo
  DOC "Armadillo root directory")
 
set(ARMADILLO_INCLUDE_DIR ${ARMADILLO_ROOT_DIR} CACHE INTERNAL "Armadillo Include Directory")

find_package_handle_standard_args(Armadillo REQUIRED_VARS ARMADILLO_ROOT_DIR )

################
# find MKL 
################

find_path(MKL_ROOT_DIR
  NAMES lib/mkl_core_dll.lib
  HINTS ${DEPENDENCIES_ROOT}/mkl
  DOC "MKL root directory")
  
find_library(MKL_CORE_LIB
  NAMES mkl_core_dll mkl_intel_lp64_dll mkl_sequential_dll
  HINTS ${MKL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

find_library(MKL_LP64_LIB
  NAMES mkl_intel_lp64_dll
  HINTS ${MKL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

find_library(MKL_SEQUENTIAL_LIB
  NAMES mkl_sequential_dll
  HINTS ${MKL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

set(MKL_LIBRARIES ${MKL_CORE_LIB} ${MKL_LP64_LIB} ${MKL_SEQUENTIAL_LIB} CACHE INTERNAL "Intel MKL Libraries")
  
set(MKL_INCLUDE_DIR ${MKL_ROOT_DIR}/include CACHE INTERNAL "MKL Include Directory")
  
find_package_handle_standard_args(mkl REQUIRED_VARS MKL_ROOT_DIR )

################
# find DAAL 
################

find_path(DAAL_ROOT_DIR
  NAMES lib/daal_core_dll.lib
  HINTS ${DEPENDENCIES_ROOT}/daal
  DOC "DAAL root directory")
    
find_library(DBB_LIB
  NAMES tbb.lib
  HINTS ${DAAL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

find_library(DBB_MALLOC_LIB
  NAMES tbbmalloc.lib
  HINTS ${DAAL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})
  
find_library(DAAL_CORE_LIB
  NAMES daal_core_dll.lib
  HINTS ${DAAL_ROOT_DIR}/lib
  PATH_SUFFIXES ${POSSIBLE_LIB_SUFFIXES})

set(DAAL_LIBRARIES ${DBB_LIB} ${DBB_MALLOC_LIB} ${DAAL_CORE_LIB} CACHE INTERNAL "Intel DAAL Libraries")
  
set(DAAL_INCLUDE_DIR ${DAAL_ROOT_DIR}/include CACHE INTERNAL "DAAL Include Directory")
  
find_package_handle_standard_args(daal REQUIRED_VARS MKL_ROOT_DIR )

set(RAPID_XML_INCLUDE_DIR ${DEPENDENCIES_ROOT}/rapidxml-1.13)
