#include "find_path.h"

#include <iostream>
#include <climits>
#include <mcpp/mcpp.h>
#include <math.h>

#include "Map.h"




Vector<mcpp::Coordinate2D> 
findPath(const Path& path,
         const Vector<Plot>& plots,
         const Plot& border,
         const mcpp::HeightMap& heightMap,
         const mcpp::Chunk& chunk,
         const Map<mcpp::Coordinate2D,
         bool>& occupied) {

    PriorityQueue<Cell> toExplore{};

    Map<mcpp::Coordinate2D, int> gScore{};
    Map<mcpp::Coordinate2D, mcpp::Coordinate2D> parent{};

    mcpp::Coordinate2D endCoord2D = path.end;

    Cell curr(path.start);
    curr.f = heuristic(path.start, path.end);
    gScore[path.start] = 0;

    toExplore.insert(curr);

    bool foundCell = false;

    while(!foundCell && !toExplore.isEmpty()) {

        curr = toExplore.pop();

        if (curr.coord == endCoord2D) {
            foundCell = true;
        }

        else {

            bool gScoreHasCurr = gScore.contains(curr.coord);

            //skipping stale entries
            bool isStale = false;
            if (gScoreHasCurr) {
                int fBestNow = gScore[curr.coord] + heuristic(curr.coord, path.end);

                
                if (curr.f > fBestNow) {
                    isStale = true;
                }
            }
            
            if (!isStale) {

                processNeighbors(curr, path, plots, border, heightMap, chunk,
                     gScore, parent, toExplore, occupied);

            }

        }

    }

    Vector<mcpp::Coordinate2D> result;

    if (!foundCell) {
        std::cout << "No path found: " <<
            path.start << " -> " << path.end << std::endl;
    }
    else {
        result = backtrack(curr, parent, path);
    }

    return result;
}


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
                      bool>& occupied) {

    bool gScoreHasCurr = gScore.contains(curr.coord);
    int gScoreCurr = gScore[curr.coord];
    
    Vector<Cell> neighbors = curr.getNeighbors();
    
    for (Cell neighbor : neighbors) {

        if (isValidCell(neighbor, curr, plots, border, heightMap, occupied)) {

            int step = calculateCost(neighbor, curr, heightMap, chunk);

            int currG = gScoreHasCurr ? gScoreCurr : INT_MAX;
            int tentativeG = (currG == INT_MAX) ? INT_MAX : currG + step;
            int bestKnown  = gScore.contains(neighbor.coord) ? gScore[neighbor.coord] : INT_MAX;

            // better path found
            if (tentativeG < bestKnown) {
                gScore[neighbor.coord] = tentativeG;
                parent[neighbor.coord] = curr.coord;

                neighbor.h = heuristic(neighbor.coord, path.end);
                neighbor.f = tentativeG + neighbor.h;

                toExplore.insert(neighbor);
            }
        }
        
    }
}



bool isValidCell(const Cell& cell,
                 const Cell& parent,
                 const Vector<Plot>& plots,
                 const Plot& border,
                 const mcpp::HeightMap& heightMap,
                 const Map<mcpp::Coordinate2D, 
                 bool>& occupied) {

    bool isViolating = false;

    if (occupied.contains(cell.coord)) {
        isViolating = true;
    }

    if (!isViolating){
        
        if (!isInBorder(cell.coord, border)) {
            isViolating = true;
        }
    }
    // just to skip checks
    if (!isViolating) {
        
        for (size_t i = 0; !isViolating && i < plots.getSize(); ++i) {
            Plot plot = plots[i];

            if (isInPlot(cell.coord, plot)) {
                isViolating = true;
            }
        }

    }

    if (!isViolating) {

        const int MAX_Y_DIFF = 4;

        try {

            int cellHeight = heightMap.get_worldspace(cell.coord);
            int parentHeight = heightMap.get_worldspace(parent.coord);
    
            if (std::abs(cellHeight - parentHeight) > MAX_Y_DIFF) {
                isViolating = true;
            }
    
        } catch (std::out_of_range& excpt) {
            isViolating = true;
        }

    }


    return isViolating ? false : true;
}

bool isInBorder(mcpp::Coordinate2D coord, 
                const Plot& plot) {
    
    int x = coord.x;
    int z = coord.z;

    return x > plot.origin.x && x < plot.bound.x &&
            z > plot.origin.z && z < plot.bound.z;
}


bool isInPlot(mcpp::Coordinate2D coord, 
              const Plot& plot) {

    int x = coord.x;
    int z = coord.z;

    return x >= plot.origin.x && x <= plot.bound.x &&
            z >= plot.origin.z && z <= plot.bound.z;
}


int calculateCost(const Cell& cell,
                  const Cell& parent, 
                  const mcpp::HeightMap& heightMap, 
                  const mcpp::Chunk& chunk) {

    const int DEFAULT_STEP = 1;
    const int HEIGHT_PENALTY = 2;
    const int WATER_PENALTY = 5;
    const int VIOLATION_PENALTY = 1 << 15;

    bool isViolating = false;

    int total = DEFAULT_STEP;

    try {

        int cellHeight = heightMap.get_worldspace(cell.coord);
        int parentHeight = heightMap.get_worldspace(parent.coord);
    
        int yDiff = std::abs(cellHeight - parentHeight);
    
        total += yDiff * HEIGHT_PENALTY;

        mcpp::Coordinate cellCoord(cell.coord.x, cellHeight, cell.coord.z);

        mcpp::BlockType cellBlock = chunk.get_worldspace(cellCoord);

        if (cellBlock == mcpp::Blocks::STILL_WATER ||
                 cellBlock == mcpp::Blocks::FLOWING_WATER) {
            total += WATER_PENALTY;
        }

    } catch(std::out_of_range& excpt) {
        isViolating = true;
    }
    

    return isViolating ? VIOLATION_PENALTY : total;
}


Vector<mcpp::Coordinate2D> 
backtrack(Cell& lastCellFound,
          Map<mcpp::Coordinate2D, mcpp::Coordinate2D>& parent,
          const Path& path) {

    Vector<mcpp::Coordinate2D> result;
    mcpp::Coordinate2D curr = lastCellFound.coord;


    while (curr != path.start) {
        result.push_back(curr);
        curr = parent[curr];
    }

    result.push_back(path.start);

    // reverse order to make it: start->goal
    size_t resultSize = result.getSize();
    for (size_t i = 0; i < resultSize / 2; ++i) {
        auto temp = result[i];
        result[i] = result[resultSize - 1 - i];
        result[resultSize - 1 - i] = temp;
    }
    

    return result;
}

int heuristic(const mcpp::Coordinate2D& a,
              const mcpp::Coordinate2D& b) {

    int dx = std::abs(a.x - b.x);
    int dz = std::abs(a.z - b.z);

    return dx + dz;
}


