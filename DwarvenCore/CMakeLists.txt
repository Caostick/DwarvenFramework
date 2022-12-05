cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarvenCore

	SOURCES
		Include/DwarvenCore/*.h
		Include/DwarvenCore/*.inl
		Include/DwarvenCore/Math/*.h
		Include/DwarvenCore/Math/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
)