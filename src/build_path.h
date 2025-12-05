#ifndef BUILD_PATH_H
#define BUILD_PATH_H

#include "Vector.h"
#include <mcpp/mcpp.h>

/**
 * @brief Build a gravel path along planned 2D coordinates.
 *
 * Projects points to their highest Y using the height map, then adjusts
 * terrain by carving or filling as needed. Handles small elevation
 * changes and water, ensuring a continuous gravel path.
 *
 * @param plan Ordered 2D coordinates from start -> end.
 * @param heightMap Height data used to find top Y at each (x,z).
 * @param chunk Block data used to detect water and air.
 */
void buildPath(const Vector<mcpp::Coordinate2D>& plan,
               const mcpp::HeightMap& heightMap,
               const mcpp::Chunk& chunk);

/* ------------------------------------------
 * ------------ Helper functions ------------
 * ------------------------------------------ */

/**
 * @brief Convert 2D coordinates to their surface world coordinates.
 *
 * For each (x,z), queries @p heightMap for the highest Y (terrain top).
 * Returns an empty vector if height lookup fails.
 *
 * @param vec Input 2D path coordinates.
 * @param heightMap World height data for (x,z) -> y lookup.
 * @return 3D coordinates positioned on terrain surface.
 */
Vector<mcpp::Coordinate>
toHeighestCoords(const Vector<mcpp::Coordinate2D>& vec,
                 const mcpp::HeightMap& heightMap);

/**
 * @brief Carve downward from @p prevCoord to @p newCoord.
 *
 * Removes blocks to reduce elevation differences, effectively cutting
 * the terrain to level the path.
 *
 * @param prevCoord Higher coordinate.
 * @param newCoord Lower coordinate (target of cut).
 * @param mc the minecraft connection.
 */
void cutDown(const mcpp::Coordinate& prevCoord,
             const mcpp::Coordinate& newCoord,
             mcpp::MinecraftConnection& mc);

/**
 * @brief Fill upward from @p prevCoord to @p newCoord.
 *
 * Adds dirt or similar blocks to raise terrain for smoother pathing.
 *
 * @param prevCoord Lower coordinate.
 * @param newCoord Higher coordinate (target of fill).
 * @param mc the minecraft connection.
 */
void buildUp(const mcpp::Coordinate& prevCoord,
             const mcpp::Coordinate& newCoord,
             mcpp::MinecraftConnection& mc);

/**
 * @brief Place support below gravel at @p currCoord.
 *
 * Ensures gravel has a solid block beneath it (e.g., dirt) when over
 * air or water.
 *
 * @param currCoord Coordinate requiring gravel support.
 * @param mc the minecraft connection.
 */
void supportGravel(const mcpp::Coordinate& currCoord,
                   mcpp::MinecraftConnection& mc);

#endif