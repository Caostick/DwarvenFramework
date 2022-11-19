cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME DwarvenFramework)

project(${PROJECT_NAME})

add_subdirectory(DwarvenCore)

add_subdirectory(DwarfPlatform)
add_subdirectory(DwarfResources)
add_subdirectory(DwarfInput)
add_subdirectory(DwarfRender)
add_subdirectory(DwarfConstructorRender)