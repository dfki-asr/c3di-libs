macro(CopyDlls)
    # copy necessary dependency DLLs to destination folder
    if(MSVC)
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug/")
        add_custom_command(
            TARGET ${PROJECT_NAME} 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${FREEIMAGE_DEBUG_DIR}/FreeImaged.dll" 
                "${FREEIMAGE_DEBUG_DIR}/FreeImagePlusd.dll" 
                "${CMAKE_BINARY_DIR}/bin/Debug/"
        )
        
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release/")
        add_custom_command(
            TARGET ${PROJECT_NAME} 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${FREEIMAGE_RELEASE_DIR}/FreeImage.dll" 
            "${FREEIMAGE_RELEASE_DIR}/FreeImagePlus.dll" 
            "${CMAKE_BINARY_DIR}/bin/Release/"
        )
        
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/")
        add_custom_command(
            TARGET ${PROJECT_NAME} 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${FREEIMAGE_RELEASE_DIR}/FreeImage.dll" 
            "${FREEIMAGE_RELEASE_DIR}/FreeImagePlus.dll" 
            "${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/"
        )

    endif(MSVC)
endmacro()