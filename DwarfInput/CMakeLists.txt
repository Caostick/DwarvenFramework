cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfInput

	SOURCES
		Include/DwarfInput/*.h
		Include/DwarfInput/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
		DwarfPlatform
)