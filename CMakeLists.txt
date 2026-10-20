cmake_minimum_required (VERSION 3.15)
project(Projekt-R)

file(GLOB_RECURSE ProjektR_Sources "${CMAKE_SOURCE_DIR}/src/model/*.cpp")
add_executable(Projekt-R ${ProjektR_Sources})
