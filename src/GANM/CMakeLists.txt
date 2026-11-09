cmake_minimum_required(VERSION 3.15)
project(GANM LANGUAGES CXX)

file(GLOB HEADER_FILES "include/")

add_executable(GANM src/main.cpp ${HEADER_FILES})

target_include_directories(GenAlgo PRIVATE include)