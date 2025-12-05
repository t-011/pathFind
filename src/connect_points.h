#ifndef CONNECT_POINTS_H
#define CONNECT_POINTS_H

#include "build_path.h"
#include "find_path.h"
#include <mcpp/mcpp.h>

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

/**
 * @brief Connect unconnected points to a growing set.
 *
 * Iteratively links the nearest unconnected point to the connected
 * set using A*, builds gravel paths, and optionally logs progress.
 *
 * @param connected Points already linked (will grow).
 * @param unconnected Points still unlinked (will shrink).
 * @param plots Plot obstacles to avoid during pathfinding.
 * @param border Border of the village.
 * @param houseToWaypoint True for house→waypoint linking mode.
 * @param isTest True to print route debug info.
 * @return Returns a std::vector of isolated points
 */
std::vector<mcpp::Coordinate> 
connectPoints(std::vector<mcpp::Coordinate>& connected,
              std::vector<mcpp::Coordinate>& unconnected,
              const std::vector<Plot>& plots,
              const Plot& border,
              bool houseToWaypoint,
              bool isTest);

/* ------------------------------------------
 * ------------ Helper functions ------------
 * ------------------------------------------ */

/**
 * @brief Find the closest pair between connected and unconnected sets.
 *
 * Uses Manhattan distance to find the smallest pair of points where
 * one belongs to the connected set and the other to the unconnected set.
 *
 * @param connected Already connected set of points.
 * @param unconnected Unconnected points to be linked.
 * @return Pair {inUnconnected, inConnected} with minimum distance.
 */
std::pair<mcpp::Coordinate, mcpp::Coordinate>
closestLink(std::vector<mcpp::Coordinate>& connected,
            std::vector<mcpp::Coordinate>& unconnected);

/**
 * @brief Compute 3D Manhattan distance between two coordinates.
 * @param firstCoord First coordinate.
 * @param secondCoord Second coordinate.
 * @return |diff x| + |diff y| + |diff z|.
 */
int getManhattanDist(const mcpp::Coordinate& firstCoord,
                     const mcpp::Coordinate& secondCoord);

/**
 * @brief Expand both coordinates outward equally on all axes.
 *
 * Used to enlarge cached region for faster height/block lookups.
 *
 * @param first One corner (modified).
 * @param second Opposite corner (modified).
 * @param incrAmount Amount to expand along x, y, and z.
 */
void increaseMargin(mcpp::Coordinate& first,
                    mcpp::Coordinate& second,
                    int incrAmount);

/**
 * @brief Print the computed 2D route for debugging.
 *
 * Displays the start and end coordinates and the sequence of
 * intermediate 2D points produced by A* pathfinding.
 *
 * @param plan Sequence of 2D points from start -> end.
 * @param path Path context (contains start and end).
 */
void printRoute(const Vector<mcpp::Coordinate2D>& plan,
                const Path& path);

/**
 * @brief Marks coordinates from the path as occupied to avoid overlap.
 * 
 * @param plan The path coordinates to register.
 * @param occupiedCoords Map tracking already used coordinates.
 */
void registerPath(const Vector<mcpp::Coordinate2D>& plan,
                  Map<mcpp::Coordinate2D, bool>& occupiedCoords);

#endif