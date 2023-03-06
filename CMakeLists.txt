cmake_minimum_required(VERSION 3.20.0)
project(trojanHorseKeylogger)
set(CMAKE_CXX_STANDARD 17)

add_executable(keylogger
    src/keylogger.cpp
    include/keylogger.hpp)

target_include_directories(keylogger 
    PUBLIC 
        include
)