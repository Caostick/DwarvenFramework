cmake_minimum_required (VERSION 3.1)

option(DwarfResources_TESTS "Enable build of tests" OFF)

if(DwarfResources_TESTS)
	add_subdirectory(Tests)
endif()

df_setup_module(
	NAME
		DwarfResources

	SOURCES
		Include/DwarfResources/*.h
		Include/DwarfResources/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
)