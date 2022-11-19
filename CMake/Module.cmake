function(df_setup_module)

	set(prefix DF)
    set(flags ENABLE_TESTS)
    set(singleValues NAME)
    set(multiValues SOURCES DEPENDENCIES THIRD_PARTY)

    include(CMakeParseArguments)
    cmake_parse_arguments(${prefix}
                     "${flags}"
                     "${singleValues}"
                     "${multiValues}"
                    ${ARGN})

	project(${DF_NAME})

	file(GLOB SRC_FILES ${DF_SOURCES})
	
	add_library (${DF_NAME} STATIC ${SRC_FILES})
	set_property(TARGET ${DF_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

	target_include_directories(${DF_NAME} PUBLIC Include/)
	
	foreach(LIB ${DF_DEPENDENCIES})
		find_library(LIB-lib ${LIB})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
	
	foreach(LIB ${DF_THIRD_PARTY})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
endfunction()