cmake_minimum_required(VERSION 3.5)

df_add_ut(
	NAME
		DwarvenTests
	SOURCES
		TestApp/App.cpp
		Tests/*.cpp
		Tests/*.h
		Tests/*.inl
		
	DEPENDENCIES
		DwarvenCore
)