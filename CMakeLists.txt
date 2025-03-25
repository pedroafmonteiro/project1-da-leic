cmake_minimum_required(VERSION 3.26)
project(project1-da-leic)

set(CMAKE_CXX_STANDARD 14)

add_executable(project1-da-leic main.cpp menu/menu.cpp menu/menu.h
        parseData/parseData.cpp
        parseData/parseData.h
        parseData/dataManager.cpp
        parseData/dataManager.h
        graph_builder/GraphBuilder.cpp
        graph_builder/GraphBuilder.h
        independent_route/independent_route.cpp
        independent_route/independent_route.h
        routing/routing.cpp
        routing/routing.h)