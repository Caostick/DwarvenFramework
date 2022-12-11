cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfIntermediate

	SOURCES
		Include/DwarfIntermediate/*.h
		Include/DwarfIntermediate/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
		DwarfFileSystem
		DwarfRender
		
	THIRD_PARTY
		CONAN_PKG::glslang
		CONAN_PKG::assimp
)