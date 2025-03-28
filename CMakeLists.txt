cmake_minimum_required(VERSION 3.26)
project(project1-da-leic)

set(CMAKE_CXX_STANDARD 14)

add_executable(project1-da-leic main.cpp menu/Menu.cpp menu/Menu.h
        parse_data/ParseData.cpp
        parse_data/ParseData.h
        parse_data/DataManager.cpp
        parse_data/DataManager.h
        graph_builder/GraphBuilder.cpp
        graph_builder/GraphBuilder.h
        routing/Routing.cpp
        routing/Routing.h)