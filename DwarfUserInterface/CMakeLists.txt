cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfUserInterface

	SOURCES
		Include/DwarfUserInterface/*.h
		Include/DwarfUserInterface/*.inl
		Include/DwarfUserInterface/Elements/*.h
		Include/DwarfUserInterface/Elements/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
		Source/Elements/*.h
		Source/Elements/*.inl
		Source/Elements/*.cpp

	DEPENDENCIES
		DwarvenCore
		DwarfRender

	THIRD_PARTY
		CONAN_PKG::stb
)