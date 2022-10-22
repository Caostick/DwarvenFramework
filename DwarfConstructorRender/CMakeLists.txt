cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME DwarfConstructorRender)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

file(GLOB RENDERER_FILES
	Include/DwarfConstructorRender/*.h
	Include/DwarfConstructorRender/*.inl
	Source/*.h
	Source/*.inl
	Source/*.cpp
)
source_group("Renderer" FILES ${RENDERER_FILES})

file(GLOB SHADER_GRAPH_FILES
	Include/DwarfConstructorRender/ShaderGraph/*.h
	Include/DwarfConstructorRender/ShaderGraph/*.inl
	Source/ShaderGraph/*.h
	Source/ShaderGraph/*.inl
	Source/ShaderGraph/*.cpp
)
source_group("Renderer\\ShaderGraph" FILES ${SHADER_GRAPH_FILES})

file(GLOB SHADER_GRAPH_NODES_FILES
	Include/DwarfConstructorRender/ShaderGraph/Nodes/*.h
	Include/DwarfConstructorRender/ShaderGraph/Nodes/*.inl
	Source/ShaderGraph/Nodes/*.h
	Source/ShaderGraph/Nodes/*.inl
	Source/ShaderGraph/Nodes/*.cpp
)
source_group("Renderer\\ShaderGraph\\Nodes" FILES ${SHADER_GRAPH_NODES_FILES})

file(GLOB RESOURCES_FILES
	Include/DwarfConstructorRender/Resources/*.h
	Include/DwarfConstructorRender/Resources/*.inl
	Source/Resources/*.h
	Source/Resources/*.inl
	Source/Resources/*.cpp
)
source_group("Renderer\\Resources" FILES ${RESOURCES_FILES})

file(GLOB LOADERS_FILES
	Include/DwarfConstructorRender/Loaders/*.h
	Include/DwarfConstructorRender/Loaders/*.inl
	Source/Loaders/*.h
	Source/Loaders/*.inl
	Source/Loaders/*.cpp
)
source_group("Renderer\\Loaders" FILES ${LOADERS_FILES})

file(GLOB API_FILES
	Source/Vulkan/*.h
	Source/Vulkan/*.inl
	Source/Vulkan/*.cpp
)
source_group("Renderer\\API" FILES ${API_FILES})

add_library (${PROJECT_NAME} STATIC
	${RENDERER_FILES}
	${SHADER_GRAPH_FILES}
	${SHADER_GRAPH_NODES_FILES}
	${RESOURCES_FILES}
	${LOADERS_FILES}
	${API_FILES}
)

include_directories(Include/)

# DwarvenCore
find_library(DwarvenCore_LIB DwarvenCore)
target_link_libraries(${PROJECT_NAME} DwarvenCore)
include_directories(../DwarvenCore/Include)

# DwarfWindowmaker
find_library(DwarfWindowmaker_LIB DwarfWindow)
target_link_libraries(${PROJECT_NAME} DwarfWindow)
include_directories(../DwarfWindow/Include)

# DwarfResourcer
find_library(DwarfResourcer_LIB DwarfResources)
target_link_libraries(${PROJECT_NAME} DwarfResources)
include_directories(../DwarfResources/Include)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

target_link_libraries(${PROJECT_NAME} CONAN_PKG::glfw)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::glslang)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::assimp)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::vulkan-headers)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::vulkan-memory-allocator)

add_definitions(-DVK_RENDER_IMPLEMENTATION)
add_definitions(-DGLFW_WINDOW_IMPLEMENTATION)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")