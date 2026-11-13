cmake_minimum_required(VERSION 3.15)
project(GANM LANGUAGES CXX)

file(GLOB HEADER_FILES "include/")

add_executable(GANM src/main.cpp ${HEADER_FILES})

target_include_directories(GANM PRIVATE include) # prominila san GenAlgo u GANM
target_compile_features(GANM PRIVATE cxx_std_17) # za optional