# Routing System

## Project Overview

A comprehensive transportation routing system implementing multiple routing algorithms with a focus on environmental friendliness. This application allows users to find optimal routes between locations using various transport modes, including a specialized "driving-walking" mode for eco-friendly routing.

## Features

- **Standard Routing**: Find the fastest route between two locations using Dijkstra's algorithm
- **Restricted Routing**: Calculate routes that avoid specific nodes or segments
- **Environmentally-Friendly Routing**: Find optimal routes that combine driving and walking to reduce emissions
- **Interactive Menu Interface**: Easy-to-use terminal interface for accessing all functionality
- **File Input/Output Support**: Process routing requests from files and save results

## Core Components

- **Graph Structure**: Template-based graph implementation with vertices and edges
- **Routing Algorithms**: Implementations of Dijkstra's algorithm and specialized routing functions
- **Menu System**: User interface handling for all routing operations
- **Parser**: Data loading and processing from various file formats
- **Graph Builder**: Construction of the routing graph from location and distance data

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

## Technical Implementation

The project uses modern C++ techniques including:

- Template metaprogramming for flexible graph structures
- Priority queues for efficient path finding
- Smart pointers for memory management
- Object-oriented design patterns

## Contributors

Design and implementation by the project team for the Algorithms and Data Structures course at FEUP.
