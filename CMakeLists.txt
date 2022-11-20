cmake_minimum_required (VERSION 3.1)

include(CMake/Module.cmake)

option(DF_ENABLE_TESTS "Enable build of tests" ON)

add_definitions(-DGLFW_WINDOW_IMPLEMENTATION)
add_definitions(-DVK_RENDER_IMPLEMENTATION)

set(CPP_VERSION 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

add_subdirectory(DwarvenCore)

add_subdirectory(DwarfPlatform)
add_subdirectory(DwarfResources)
add_subdirectory(DwarfInput)
add_subdirectory(DwarfRender)
add_subdirectory(DwarfProcedural)

if(DF_ENABLE_TESTS)
	add_subdirectory(DwarvenTests)
endif()