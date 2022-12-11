if(NOT ${DF_OUTPUT_DIR} STREQUAL "")
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${DF_OUTPUT_DIR} )
    	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${DF_OUTPUT_DIR} )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif()

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
	target_compile_definitions(${DF_NAME} PUBLIC ${DF_NAME}_Enabled=1)

	foreach(LIB ${DF_DEPENDENCIES})
		find_library(LIB-lib ${LIB})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
	
	foreach(LIB ${DF_THIRD_PARTY})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
endfunction()



function(df_setup_app)

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
	
	add_executable (${DF_NAME} ${SRC_FILES})
	set_property(TARGET ${DF_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})
	if(NOT ${DF_OUTPUT_DIR} STREQUAL "")
		set_property(TARGET ${DF_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${DF_OUTPUT_DIR})
	endif()

	target_include_directories(${DF_NAME} PRIVATE Code/)

	foreach(LIB ${DF_DEPENDENCIES})
		find_library(LIB-lib ${LIB})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
	
	foreach(LIB ${DF_THIRD_PARTY})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
endfunction()



function(df_add_ut)

	set(prefix DF)
    set(flags ENABLE_TESTS)
    set(singleValues NAME)
    set(multiValues SOURCES DEPENDENCIES)

    include(CMakeParseArguments)
    cmake_parse_arguments(${prefix}
                     "${flags}"
                     "${singleValues}"
                     "${multiValues}"
                    ${ARGN})

	project(${DF_NAME})

	file(GLOB SRC_FILES ${DF_SOURCES})
	
	add_executable(${DF_NAME} ${SRC_FILES})
	set_property(TARGET ${DF_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})
	
	foreach(LIB ${DF_DEPENDENCIES})
		find_library(LIB-lib ${LIB})
		target_link_libraries(${DF_NAME} ${LIB})
	endforeach()
	
	target_link_libraries(${DF_NAME} CONAN_PKG::catch2)
	
	add_test(NAME ${DF_NAME} COMMAND ${DF_NAME})
endfunction()