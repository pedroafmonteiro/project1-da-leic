# Design of Algorithms Project 1 - Spring 2025
> Developed by Group 2 - Class 15

## Project Overview

A path-planning tool to assist with urban navigation.

## Features

- **Independent Routing**: Finds the fastest route between two locations.
- **Restricted Routing**: FInds the fastest route between two locations with specific routing constraints.
- **Environmentally-Friendly Routing**: Finds the best (shortest overall) route for driving and walking.

## Core Components

- **Graph Structure**: Template-based graph implementation with vertices and edges. Modified to support walking or driving edges.
- **Routing Algorithms**: Implementations of Dijkstra's algorithm for the routing features.
- **Menu System**: User interface handling for all routing operations.
- **Data Management**: Handles the loading and parsing of location and distance data from CSV files.
- **Graph Builder**: Construction of the routing graph from location and distance data.

## Time Complexity

- **Dijkstra's Algorithm**: O(E log V) where E is the number of edges and V is the number of vertices
- **Environmentally-Friendly Routing**: O(P \* E log V) where P is the number of potential parking locations
- **Graph Building**: O(V + E) for constructing the graph from data

## Usage

1. Build the project using your preferred C++ compiler
2. Run the executable
3. Follow the on-screen menu to select your desired routing option
4. Enter source and destination locations, along with any constraints
5. View the results showing the optimal route and timing information
