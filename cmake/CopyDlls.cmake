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
                "${MKL_ROOT_DIR}/lib/mkl_core.dll"
                "${MKL_ROOT_DIR}/lib/mkl_sequential.dll"
                "${MKL_ROOT_DIR}/lib/mkl_avx2.dll"
                "${MKL_ROOT_DIR}/lib/mkl_def.dll"
                "${DAAL_ROOT_DIR}/bin/daal_core.dll"
                "${DAAL_ROOT_DIR}/bin/daal_thread.dll"
                "${DAAL_ROOT_DIR}/bin/tbb.dll"
                "${DAAL_ROOT_DIR}/bin/tbbmalloc.dll"
                "${CMAKE_BINARY_DIR}/bin/Debug/"
        )
        
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release/")
        add_custom_command(
            TARGET ${PROJECT_NAME} 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${FREEIMAGE_RELEASE_DIR}/FreeImage.dll" 
            "${FREEIMAGE_RELEASE_DIR}/FreeImagePlus.dll" 
            "${MKL_ROOT_DIR}/lib/mkl_core.dll"
            "${MKL_ROOT_DIR}/lib/mkl_sequential.dll"
            "${MKL_ROOT_DIR}/lib/mkl_avx2.dll"
            "${MKL_ROOT_DIR}/lib/mkl_def.dll"
            "${DAAL_ROOT_DIR}/bin/daal_core.dll"
            "${DAAL_ROOT_DIR}/bin/daal_thread.dll"
            "${DAAL_ROOT_DIR}/bin/tbb.dll"
            "${DAAL_ROOT_DIR}/bin/tbbmalloc.dll"
            "${CMAKE_BINARY_DIR}/bin/Release/"
        )
        
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/RelWithDebug/")
        add_custom_command(
            TARGET ${PROJECT_NAME} 
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${FREEIMAGE_RELEASE_DIR}/FreeImage.dll" 
            "${FREEIMAGE_RELEASE_DIR}/FreeImagePlus.dll" 
            "${MKL_ROOT_DIR}/lib/mkl_core.dll"
            "${MKL_ROOT_DIR}/lib/mkl_sequential.dll"
            "${MKL_ROOT_DIR}/lib/mkl_avx2.dll"
            "${MKL_ROOT_DIR}/lib/mkl_def.dll"
            "${DAAL_ROOT_DIR}/bin/daal_core.dll"
            "${DAAL_ROOT_DIR}/bin/daal_thread.dll"
            "${DAAL_ROOT_DIR}/bin/tbb.dll"
            "${DAAL_ROOT_DIR}/bin/tbbmalloc.dll"
            "${CMAKE_BINARY_DIR}/bin/RelWithDebug/"
        )

    endif(MSVC)
endmacro()