cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfMultithreading

	SOURCES
		Include/DwarfMultithreading/*.h
		Include/DwarfMultithreading/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
)