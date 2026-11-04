cmake_minimum_required(VERSION 3.15)
project(OsnovnaNeuronskaMreza LANGUAGES CXX)

add_executable(OsnovnaMreza main.cpp NeuralNetwork.h)
target_compile_features(OsnovnaMreza PRIVATE cxx_std_17)
set_target_properties(OsnovnaMreza PROPERTIES CXX_EXTENSIONS OFF)
