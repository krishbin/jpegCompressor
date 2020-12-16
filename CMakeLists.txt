cmake_minimum_required(VERSION 3.10)

project(jpegCompressor)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
