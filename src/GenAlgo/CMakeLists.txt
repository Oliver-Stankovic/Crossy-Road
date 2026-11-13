cmake_minimum_required(VERSION 3.15)
project(GenAlgo LANGUAGES CXX)

file(GLOB HEADER_FILES "include/funcs.h")

add_executable(GenAlgo src/mainman.cpp ${HEADER_FILES})

target_include_directories(GenAlgo PRIVATE include)
