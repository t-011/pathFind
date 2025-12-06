# PathFind

A* pathfinding API built in C++. Includes custom STL container implementations.

## Features

- A* pathfinding algorithm
- Custom vector, queue, and other STL-like containers
- Built on mcpp

## Requirements

- C++
- mcpp library
- Make

## Build
```bash
make
```

## Usage
```cpp
#include "pathfind.h"

// Find path between two points using A*
auto path = findPath(start, goal, grid);
```

## Learning Project

This project implements pathfinding and STL containers from scratch to understand data structures and algorithms at a deeper level.
