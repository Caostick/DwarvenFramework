cmake_minimum_required (VERSION 3.1)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

df_setup_module(
	NAME
		DwarfPlatform

	SOURCES
		Include/DwarfPlatform/*.h
		Include/DwarfPlatform/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore

	THIRD_PARTY
		CONAN_PKG::glfw
)