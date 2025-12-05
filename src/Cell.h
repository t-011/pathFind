#ifndef CELL_H
#define CELL_H

#include <mcpp/mcpp.h>
#include "Vector.h"
#include <climits>

/**
 * @brief Represents the four cardinal directions used for neighbor generation.
 */
enum Direction { Unknown = 0, North, South, West, East };

/**
 * @brief Represents a cell (node) used by the A* pathfinding algorithm.
 * 
 * Stores a 2D coordinate along with:
 * - f: total cost (for priority comparison)
 * - h: heuristic cost to goal
 */
class Cell {
    public:
        mcpp::Coordinate2D coord;
        int f = 0;
        int h = 0;

        /**
         * @brief Constructs a cell with given coordinate and costs.
         * @param coord The cell’s coordinate (default: (0,0)).
         * @param f Total cost (default: INT_MAX).
         * @param h Heuristic cost (default: INT_MAX).
         */
        Cell(mcpp::Coordinate2D coord = mcpp::Coordinate2D(),
            int f = INT_MAX, int h = INT_MAX);

        /**
         * @brief Returns the four neighboring cells (N, S, E, W).
         * @return Vector of neighboring cells.
         */
        Vector<Cell> getNeighbors() const;

        /**
         * @brief Comparison operator for min-heap ordering (used by A*).
         * @param other Cell to compare with.
         * @return True if this cell has lower total cost.
         */
        bool operator<(const Cell&) const;

        /**
         * @brief Comparison operator for inverse ordering.
         * @param other Cell to compare with.
         * @return True if this cell has higher total cost.
         */
        bool operator>(const Cell&) const;

    private:
        /**
         * @brief Returns the neighboring cell in a given direction.
         * @param direction Direction to move toward.
         * @return The resulting neighboring cell.
         */
        Cell neighborIn(Direction) const;
};

#endif