cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME DwarfInput)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

file(GLOB SOURCES
	Include/DwarfInput/*.h
	Include/DwarfInput/*.inl
	Source/*.h
	Source/*.inl
	Source/*.cpp
)

add_library (${PROJECT_NAME} STATIC ${SOURCES})

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

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

add_definitions(-DGLFW_WINDOW_IMPLEMENTATION)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")