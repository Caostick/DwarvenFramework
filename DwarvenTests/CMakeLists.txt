cmake_minimum_required(VERSION 3.5)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

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