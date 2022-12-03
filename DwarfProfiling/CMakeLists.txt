cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfProfiling

	SOURCES
		Include/DwarfProfiling/*.h
		Include/DwarfProfiling/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
		
	THIRD_PARTY
		CONAN_PKG::optick
)