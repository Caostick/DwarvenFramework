cmake_minimum_required (VERSION 3.1)

option(DF_RESOURCER_TESTS "Enable build of tests" OFF)

set(PROJECT_NAME DwarfResources)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

file(GLOB SOURCES
	Include/DwarfResourcer/*.h
	Include/DwarfResourcer/*.inl
	Source/*.h
	Source/*.inl
	Source/*.cpp
)

add_library (${PROJECT_NAME} STATIC ${SOURCES})

include_directories(Include/)

find_library(DwarvenCore_LIB DwarvenCore)
target_link_libraries(${PROJECT_NAME} DwarvenCore)
include_directories(../DwarvenCore/Include)


set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "DwarfResources")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

if(DF_RESOURCER_TESTS)
add_subdirectory(Tests)
endif()