if(MSVC)
	add_definitions(/wd4482) # disable "nonstandard extension used" which will disapear in newer compilers anyways
	add_definitions(/wd4275) # disable "non – DLL-interface classkey used as base for DLL-interface classkey" which is not an issue unless accessing static data
	add_definitions(/wd4251) # disable "class X needs to have dll-interface to be used by clients of class Y" which is no issue unless using static data
	add_definitions(/wd4996) # disable "class X needs to have dll-interface to be used by clients of class Y" which is no issue unless using static data
	add_definitions(/wd4595) # disable "'operator new': non-member operator new or delete functions may not be declared inline", which occurs from VS 2015 migration. We cannot declare them non-inline and move inside .cpp file.
	add_definitions(/wd4834) # disable "discarding return value of function with 'nodiscard' attribute
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_WARNINGS")  
    set(CMAKE_CXX_STANDARD 17)	
	if (MSVC)
		add_compile_options("$<$<CONFIG:RELEASE>:/Ox>")
		add_compile_options("$<$<CONFIG:RELEASE>:/Ob2>")
		add_compile_options("$<$<CONFIG:RELEASE>:/arch:AVX>")
		add_compile_options("$<$<CONFIG:RELEASE>:/fp:fast>")
	endif()
endif(MSVC)

