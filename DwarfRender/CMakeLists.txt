cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME DwarfRender)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

file(GLOB RENDER_FILES
	Include/DwarfRender/*.h
	Include/DwarfRender/*.inl
	Source/*.h
	Source/*.inl
	Source/*.cpp
)
source_group("Render" FILES ${RENDER_FILES})

file(GLOB RENDER_VK_FILES
	Source/Vulkan/*.h
	Source/Vulkan/*.inl
	Source/Vulkan/*.cpp
)
source_group("Render\\Vulkan" FILES ${RENDER_VK_FILES})

add_library (${PROJECT_NAME} STATIC
	${RENDER_FILES}
	${RENDER_VK_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC Include/)

# DwarvenCore
find_library(DwarvenCore_LIB DwarvenCore)
target_link_libraries(${PROJECT_NAME} DwarvenCore)

# DwarfPlatform
find_library(DwarfPlatform_LIB DwarfPlatform)
target_link_libraries(${PROJECT_NAME} DwarfPlatform)

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