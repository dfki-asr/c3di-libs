# Find FreeImagePlus
# Source: https://github.com/drakeguan/cmake_modules/blob/master/FindFreeImagePlus.cmake

IF (WIN32)
    set(FREEIMAGE_ROOT_DIR "${C3DI_LIBS_DEPENDENCIES_ROOT}/FreeImage/Windows/")
    FIND_PATH( FREEIMAGEPLUS_INCLUDE_PATH FreeImagePlus.h
        ${FREEIMAGE_ROOT_DIR}/include
        ${FREEIMAGE_ROOT_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/FreeImage/include
        DOC "The directory where FreeImagePlus.h resides")
    FIND_LIBRARY( FREEIMAGEPLUS_LIBRARY
        NAMES FreeImagePlus freeimageplus
        PATHS
        ${FREEIMAGE_ROOT_DIR}/x64
        ${FREEIMAGE_ROOT_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/FreeImage/lib/x64
        DOC "The FreeImagePlus library")
ELSE (WIN32)
   # Find include files
    find_path(
        FREEIMAGEPLUS_INCLUDE_PATH
        NAMES FreeImagePlus.h
        PATHS
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        DOC "The directory where FreeImage.h resides")

    # Find library files
    find_library(
        FREEIMAGEPLUS_LIBRARY
        NAMES freeimageplus
        PATHS
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /usr/lib/x86_64-linux-gnu
            /opt/local/lib
            ${FREEIMAGE_ROOT_DIR}/lib
        DOC "The FreeImage library")
ENDIF (WIN32)

SET(FREEIMAGEPLUS_LIBRARIES ${FREEIMAGEPLUS_LIBRARY})
IF (FREEIMAGEPLUS_INCLUDE_PATH AND FREEIMAGEPLUS_LIBRARY) 
        SET( FREEIMAGEPLUS_FOUND TRUE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
        message("Found FreeImagePlus")
ELSE (FREEIMAGEPLUS_INCLUDE_PATH AND FREEIMAGEPLUS_LIBRARY)
        SET( FREEIMAGEPLUS_FOUND FALSE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
        message("FreeImagePlus NOT FOUND")    
ENDIF (FREEIMAGEPLUS_INCLUDE_PATH AND FREEIMAGEPLUS_LIBRARY)

MARK_AS_ADVANCED(
        FREEIMAGEPLUS_FOUND
        FREEIMAGEPLUS_LIBRARY
        FREEIMAGEPLUS_LIBRARIES
        FREEIMAGEPLUS_INCLUDE_PATH)
