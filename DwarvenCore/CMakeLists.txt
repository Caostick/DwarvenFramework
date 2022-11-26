cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarvenCore

	SOURCES
		Include/DwarvenCore/*.h
		Include/DwarvenCore/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
		Include/DwarvenCore/Math/*.h
		Include/DwarvenCore/Math/*.inl
		Include/DwarvenCore/Math/*.cpp
		Include/DwarvenCore/Application/*.h
		Include/DwarvenCore/Application/*.inl
		Source/Application/*.h
		Source/Application/*.inl
		Source/Application/*.cpp
)