macro(CopyDlls)
    # Copying dependent DLLs to executable directory after building the target on Windows
    if(MSVC)
        add_custom_command(TARGET ${LIBMMV_UNIT_TEST_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy
                                  "${FREEIMAGE_ROOT_DIR}/lib/x64/FreeImage.dll"
                                  "${FREEIMAGE_ROOT_DIR}/lib/x64/FreeImagePlus.dll"
                                  "${CMAKE_BINARY_DIR}/bin/$<CONFIG>/")
    endif(MSVC)
endmacro()