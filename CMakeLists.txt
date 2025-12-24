cmake_minimum_required(VERSION 3.16)
project(mlparser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mlparser
    src/main.cpp
    src/parse.cpp
    src/parse-util.cpp
    src/parse-test.cpp
    )

target_include_directories(mlparser PRIVATE ${CMAKE_SOURCE_DIR})