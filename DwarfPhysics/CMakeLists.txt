cmake_minimum_required (VERSION 3.1)

df_setup_module(
	NAME
		DwarfPhysics

	SOURCES
		Include/DwarfPhysics/*.h
		Include/DwarfPhysics/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp

	DEPENDENCIES
		DwarvenCore

	THIRD_PARTY
		CONAN_PKG::bullet3
)