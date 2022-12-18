cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfScene

	SOURCES
		Include/DwarfScene/*.h
		Include/DwarfScene/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
)