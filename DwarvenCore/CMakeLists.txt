cmake_minimum_required (VERSION 3.1)

option(DwarvenCore_TESTS "Enable build of tests" OFF)

set(PROJECT_NAME DwarvenCore)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

file(GLOB CORE_FILES
	Include/DwarvenCore/*.h
	Include/DwarvenCore/*.inl
	Source/*.h
	Source/*.inl
	Source/*.cpp
)
source_group("Core" FILES ${CORE_FILES})

file(GLOB MATH_FILES
	Include/DwarvenCore/Math/*.h
	Include/DwarvenCore/Math/*.inl
	Include/DwarvenCore/Math/*.cpp
)
source_group("Core\\Math" FILES ${MATH_FILES})

file(GLOB APP_FILES
	Include/DwarvenCore/Application/*.h
	Include/DwarvenCore/Application/*.inl
	Source/Application/*.h
	Source/Application/*.inl
	Source/Application/*.cpp
)
source_group("Core\\Application" FILES ${APP_FILES})

add_library (${PROJECT_NAME} STATIC
	${CORE_FILES}
	${MATH_FILES}
	${APP_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC Include/)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

if(DwarvenCore_TESTS)
	add_subdirectory(Tests)
endif()