#ifndef FIND_PATH_H
#define FIND_PATH_H

#include <mcpp/mcpp.h>

#include "Vector.h"
#include "PriorityQueue.h"
#include "Cell.h"
#include "Map.h"

#include "../paths.h"
#include "../plots.h"

/**
 * @brief Compute a path using A* on a heightmap while avoiding plots.
 *
 * Expands from @p path.start toward @p path.end. Steep steps and water
 * are penalized. Any coordinate inside @p plots is treated as blocked.
 * Returns start -> end coordinates if found; otherwise an empty vector.
 *
 * @param path Path descriptor (uses start/end; not modified).
 * @param plots Plots to avoid (obstacles).
 * @param border Border of the village.
 * @param heightMap World height data for slope/validity checks.
 * @param chunk Block data to detect water and surface blocks.
 * @param occupied Map tracking used path coordinates to prevent overlaps.
 * @return 2D coordinates from start to goal, or empty if none.
 */
Vector<mcpp::Coordinate2D>
findPath(const Path& path,
         const Vector<Plot>& plots,
         const Plot& border,
         const mcpp::HeightMap& heightMap,
         const mcpp::Chunk& chunk,
         const Map<mcpp::Coordinate2D, 
         bool>& occupied);

/**
 * @brief Expand @p curr's neighbors and update A* scores.
 *
 * For each valid neighbor: compute step cost, update g/f, set parent,
 * and push into the open set if the path improves.
 *
 * @param curr Cell being expanded.
 * @param path Path context (provides the goal/end).
 * @param plots Plots to avoid when validating cells.
 * @param border Border of the village.
 * @param heightMap Terrain data for slope checks.
 * @param chunk Block data for water/surface checks.
 * @param gScore Map of g-costs (cost-from-start) by coordinate.
 * @param parent Came-from map: child coord -> parent coord.
 * @param toExplore Open set (min-heap) used by A*.
 * @param occupied Map tracking used path coordinates to prevent overlaps.
 */
void processNeighbors(Cell& curr,
                      const Path& path,
                      const Vector<Plot>& plots,
                      const Plot& border,
                      const mcpp::HeightMap& heightMap,
                      const mcpp::Chunk& chunk,
                      Map<mcpp::Coordinate2D, int>& gScore,
                      Map<mcpp::Coordinate2D, mcpp::Coordinate2D>& parent,
                      PriorityQueue<Cell>& toExplore,
                      const Map<mcpp::Coordinate2D, 
                      bool>& occupied);

/* ------------------------------------------
 * ------------ Helper functions ------------
 * ------------------------------------------ */

/**
 * @brief Check a neighbor against plot and terrain constraints.
 *
 * Verifies plot exclusion, world bounds, and max allowed steepness from
 * @p parent to @p cell. And checks that cell.coord isn't already occupied
 *
 * @param cell Candidate neighbor to evaluate.
 * @param parent Current cell from which @p cell is reached.
 * @param plots Areas to avoid (blocked).
 * @param border Border of the village.
 * @param heightMap Height lookup for slope and bounds.
 * @param occupied Map tracking used path coordinates to prevent overlaps.
 * @return true if traversable; false otherwise.
 */
bool isValidCell(const Cell& cell,
                 const Cell& parent,
                 const Vector<Plot>& plots,
                 const Plot& border,
                 const mcpp::HeightMap& heightMap,
                 const Map<mcpp::Coordinate2D, 
                 bool>& occupied);

/**
 * @brief Checks if a coordinate lies strictly inside the village border area.
 *
 * Excludes the outermost wall coordinates — points on the exact border edges 
 * (origin/bound) are considered outside. This prevents building paths on the 
 * village wall itself.
 *
 * @param coord Coordinate to test.
 * @param plot  The border plot defining the village bounds.
 * @return true if strictly inside the border (not on the wall);
 *   false otherwise.
 */
bool isInBorder(mcpp::Coordinate2D coord, 
                const Plot& plot);

/**
 * @brief Checks if a coordinate lies inside or on a house plot.
 *
 * Inclusive of the plot’s boundary, coordinates on the edges count as inside. 
 * This ensures that no path or structure 
 * overlaps with or touches a building plot.
 *
 * @param coord Coordinate to test.
 * @param plot  The plot area to check against.
 * @return true if within or on the plot boundary; false otherwise.
 */
bool isInPlot(mcpp::Coordinate2D coord, 
              const Plot& plot);

/**
 * @brief Step cost from @p parent to @p cell with penalties.
 *
 * Base step is 1, plus slope and water penalties. Returns a very large
 * value if invalid/out of bounds to effectively discard the move.
 *
 * @param cell Destination cell.
 * @param parent Source cell.
 * @param heightMap Height data for slope calculation.
 * @param chunk Block data for water detection.
 * @return Non-negative movement cost; large value for violations.
 */
int calculateCost(const Cell& cell,
                  const Cell& parent,
                  const mcpp::HeightMap& heightMap,
                  const mcpp::Chunk& chunk);

/**
 * @brief Reconstruct path by following @p parent from goal to start.
 *
 * Produces a start->goal ordered sequence.
 *
 * @param foundLastCell Goal cell reached by the search.
 * @param parent Map from child coord -> parent coord.
 * @param path Path context (provides start).
 * @return Coordinates from start to goal (inclusive).
 */
Vector<mcpp::Coordinate2D>
backtrack(Cell& foundLastCell,
          Map<mcpp::Coordinate2D, mcpp::Coordinate2D>& parent,
          const Path& path);

/**
 * @brief Manhattan heuristic between two 2D coordinates.
 * @param a First coordinate.
 * @param b Second coordinate.
 * @return |diff x| + |diff z|.
 */
int heuristic(const mcpp::Coordinate2D& a,
              const mcpp::Coordinate2D& b);


// Simple hash for Coord2D in Map
namespace std {
    template<>
    struct hash<mcpp::Coordinate2D> {

        size_t operator()(const mcpp::Coordinate2D& c) const noexcept {

            uint64_t x = static_cast<uint32_t>(c.x);
            uint64_t z = static_cast<uint32_t>(c.z);
            uint64_t h = x * 0x9E3779B185EBCA87ull ^
                (z + 0x9E3779B185EBCA87ull + (x<<6) + (x>>2));
                
            return static_cast<size_t>(h);
        }
    };
}

#endif