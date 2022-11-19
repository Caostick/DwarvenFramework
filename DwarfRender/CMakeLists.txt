cmake_minimum_required (VERSION 3.1)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

df_setup_module(
	NAME
		DwarfRender

	SOURCES
		Include/DwarfRender/*.h
		Include/DwarfRender/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
		Source/Vulkan/*.h
		Source/Vulkan/*.inl
		Source/Vulkan/*.cpp

	DEPENDENCIES
		DwarvenCore
		DwarfPlatform

	THIRD_PARTY
		CONAN_PKG::glfw
		CONAN_PKG::glslang
		CONAN_PKG::assimp
		CONAN_PKG::vulkan-headers
		CONAN_PKG::vulkan-memory-allocator
)