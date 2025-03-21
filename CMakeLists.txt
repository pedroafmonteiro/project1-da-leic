cmake_minimum_required(VERSION 3.26)
project(project1-da-leic)

set(CMAKE_CXX_STANDARD 14)

add_executable(project1-da-leic main.cpp menu/menu.cpp menu/menu.h
        readData/readData.cpp
        readData/readData.h)