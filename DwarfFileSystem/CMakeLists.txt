cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfFileSystem

	SOURCES
		Include/DwarfFileSystem/*.h
		Include/DwarfFileSystem/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore
)