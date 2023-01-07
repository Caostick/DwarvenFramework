cmake_minimum_required (VERSION 3.1)

include(CMake/Module.cmake)

include(ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

option(DF_ENABLE_TESTS "Enable build of tests" ON)

add_definitions(-DGLFW_WINDOW_IMPLEMENTATION)
add_definitions(-DVK_RENDER_IMPLEMENTATION)
add_definitions(-DPLATFORM_WINDOWS)

set(CPP_VERSION 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

add_subdirectory(DwarvenCore)

add_subdirectory(DwarfPlatform)
add_subdirectory(DwarfProfiling)
add_subdirectory(DwarfPhysics)
add_subdirectory(DwarfMultithreading)
add_subdirectory(DwarfIntermediate)
add_subdirectory(DwarfFileSystem)
add_subdirectory(DwarfResources)
add_subdirectory(DwarfInput)
add_subdirectory(DwarfRender)
add_subdirectory(DwarfProcedural)
add_subdirectory(DwarfScene)
add_subdirectory(DwarfUserInterface)

if(DF_ENABLE_TESTS)
	add_subdirectory(DwarvenTests)
endif()