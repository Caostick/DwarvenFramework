cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfProcedural

	SOURCES
		Include/DwarfProcedural/*.h
		Include/DwarfProcedural/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
)