# Module for locating Dependencies.
#
# Customizable variables:
#   DEPENDENCIES_ROOT
#

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#	message(STATUS "CMake thinks this is 64-bit environment")
	set(POSSIBLE_LIB_SUFFIXES Win64 x64 x86_64 lib/Win64 lib/x86_64 lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
#	message(STATUS "CMake thinks this is 32-bit environment")
	set(POSSIBLE_LIB_SUFFIXES Win32 x86 lib/Win32 lib/x86)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

find_path(C3DI_LIBS_DEPENDENCIES_ROOT
  NAMES "dependencies_package.txt"
  HINTS "dependencies" "../c3di_libs_dependencies" "../../c3di_libs_dependencies"
  DOC "dependencies root directory")
  message(STATUS "dependencies root: ${C3DI_LIBS_DEPENDENCIES_ROOT}")

################
# find FreeImage
################
find_package(FreeImage REQUIRED)


################
# find RAPID_XML 
################

set(RAPID_XML_INCLUDE_DIR ${C3DI_LIBS_DEPENDENCIES_ROOT}/rapidxml-1.13)
