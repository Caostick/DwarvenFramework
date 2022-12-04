cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfResources

	SOURCES
		Include/DwarfResources/*.h
		Include/DwarfResources/*.inl
		Include/DwarfResources/Loaders/*.h
		Include/DwarfResources/Loaders/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
		Source/Loaders/*.h
		Source/Loaders/*.inl
		Source/Loaders/*.cpp
		
	DEPENDENCIES
		DwarvenCore
		DwarfFileSystem
		DwarfMultithreading
)