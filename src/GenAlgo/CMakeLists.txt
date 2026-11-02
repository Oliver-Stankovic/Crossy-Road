cmake_minimum_required(VERSION 3.15)
project(GenAlgo LANGUAGES CXX)

file(GLOB HEADER_FILES "include/*.h")

add_executable(GenAlgo src/main.cpp ${HEADER_FILES})

target_include_directories(GenAlgo PRIVATE include)
