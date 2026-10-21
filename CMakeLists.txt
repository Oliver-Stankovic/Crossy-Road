cmake_minimum_required (VERSION 3.15)
project(Projekt-R)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE ProjektR_Sources "${CMAKE_SOURCE_DIR}/src/model/*.cpp" "${CMAKE_SOURCE_DIR}/src/gui/*.cpp")
add_executable(Projekt-R ${ProjektR_Sources})
target_compile_features(Projekt-R PRIVATE cxx_std_17)
target_link_libraries(Projekt-R PRIVATE SFML::Graphics SFML::Window SFML::System)