cmake_minimum_required (VERSION 3.1)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

df_setup_module(
	NAME
		DwarfConstructorRender

	SOURCES
		Include/DwarfConstructorRender/*.h
		Include/DwarfConstructorRender/*.inl
		Include/DwarfConstructorRender/ShaderGraph/*.h
		Include/DwarfConstructorRender/ShaderGraph/*.inl
		Include/DwarfConstructorRender/ShaderGraph/Nodes/*.h
		Include/DwarfConstructorRender/ShaderGraph/Nodes/*.inl
		Include/DwarfConstructorRender/Resources/*.h
		Include/DwarfConstructorRender/Resources/*.inl
		Include/DwarfConstructorRender/Loaders/*.h
		Include/DwarfConstructorRender/Loaders/*.inl
		Source/*.h
		Source/*.inl
		Source/*.cpp
		Source/ShaderGraph/*.h
		Source/ShaderGraph/*.inl
		Source/ShaderGraph/*.cpp
		Source/ShaderGraph/Nodes/*.h
		Source/ShaderGraph/Nodes/*.inl
		Source/ShaderGraph/Nodes/*.cpp
		Source/Resources/*.h
		Source/Resources/*.inl
		Source/Resources/*.cpp
		Source/Loaders/*.h
		Source/Loaders/*.inl
		Source/Loaders/*.cpp
		Source/Vulkan/*.h
		Source/Vulkan/*.inl
		Source/Vulkan/*.cpp

	DEPENDENCIES
		DwarvenCore
		DwarfPlatform
		DwarfResources

	THIRD_PARTY
		CONAN_PKG::glfw
		CONAN_PKG::glslang
		CONAN_PKG::assimp
		CONAN_PKG::vulkan-headers
		CONAN_PKG::vulkan-memory-allocator
)