#include "connect_points.h"
#include "Map.h"
#include <climits>




std::vector<mcpp::Coordinate> 
connectPoints(std::vector<mcpp::Coordinate>& connected,
              std::vector<mcpp::Coordinate>& unconnected,
              const std::vector<Plot>& stdPlots,
              const Plot& border,
              bool houseToWaypoint,
              bool isTest) {

    std::vector<mcpp::Coordinate> isolated{};

    if (!connected.empty()) {

        Vector<Plot> plots = stdPlots;
    
        const int SIDE_INCREASE = 20;
        mcpp::MinecraftConnection mc;

        Map<mcpp::Coordinate2D, bool> occupied{};
        
        while (!unconnected.empty()) {
            auto link = closestLink(connected, unconnected);
    
            Path path{};
    
            //We begin from unconnected to the connected (House -> Waypoint)
            path.start = link.first;
            path.end = link.second;
    
            mcpp::Coordinate firstCachePos(path.start);
            mcpp::Coordinate secondCachePos(path.end);
    
            int dist = getManhattanDist(firstCachePos, secondCachePos);
    
            increaseMargin(firstCachePos, secondCachePos,
                 std::max(SIDE_INCREASE, dist / SIDE_INCREASE));
    
            //Caches, for fast lookups
            mcpp::HeightMap heightMap = 
                 mc.getHeights(firstCachePos, secondCachePos);
            mcpp::Chunk chunk = 
                 mc.getBlocks(firstCachePos, secondCachePos);
            
            Vector<mcpp::Coordinate2D> plan = 
                 findPath(path, plots, border, heightMap, chunk, occupied);
    
            if (plan.getSize() > 0) {
    
                if (!houseToWaypoint) {
                    connected.push_back(path.start);
                }
    
                buildPath(plan, heightMap, chunk);
                registerPath(plan, occupied);
            }
            else {
                isolated.push_back(path.start);
            }
    
            if (isTest) {
                printRoute(plan, path);
            }
            
            //remove path.start, for both cases (found path or didn't find path)
            unconnected.erase(std::remove(unconnected.begin(), 
                 unconnected.end(), path.start), unconnected.end());
    
        }

    }

    return isolated;

}





std::pair<mcpp::Coordinate, mcpp::Coordinate> 
closestLink(std::vector<mcpp::Coordinate>& connected,
            std::vector<mcpp::Coordinate>& unconnected) {

    std::pair<mcpp::Coordinate, mcpp::Coordinate> vertices{};
    int minDist = INT_MAX;
    
    for (mcpp::Coordinate& connNode : connected) {
        for (mcpp::Coordinate& unconnNode : unconnected) {

            int dist = getManhattanDist(connNode, unconnNode);

            if (dist < minDist) {
                minDist = dist;

                vertices.first = unconnNode;
                vertices.second = connNode;
            }

        }
    }
    

    return vertices;
}


int getManhattanDist(const mcpp::Coordinate& firstCoord, 
                     const mcpp::Coordinate& secondCoord) {
                        
    int dist = 0;

    dist += std::abs(firstCoord.x - secondCoord.x);
    dist += std::abs(firstCoord.y - secondCoord.y);
    dist += std::abs(firstCoord.z - secondCoord.z);


    return dist;
}


void increaseMargin(mcpp::Coordinate& first, 
                    mcpp::Coordinate& second, 
                    int incrAmount) {
    
    //X
    if (first.x < second.x) {
        first.x -= incrAmount;
        second.x += incrAmount;
    }
    else {
        second.x -= incrAmount;
        first.x += incrAmount;
    }


    //Z
    if (first.z < second.z) {
        first.z -= incrAmount;
        second.z += incrAmount;
    }
    else {
        second.z -= incrAmount;
        first.z += incrAmount;
    }


    //Y
    if (first.y < second.y) {
        first.y -= incrAmount;
        second.y += incrAmount;
    }
    else {
        second.y -= incrAmount;
        first.y += incrAmount;
    }

    return;
}


void printRoute(const Vector<mcpp::Coordinate2D>& plan, 
                const Path& path) {

    if (plan.getSize() > 0) {

        std::ostringstream oSS;
        oSS << "Route {" << path.start << " -> " << path.end << "}: ";


        size_t planSize = plan.getSize();

        oSS << "[ ";
        for (size_t i = 0; i < planSize; ++i) {
            if (i != planSize - 1) {
                oSS << plan[i] << " ";
            }
            else {
                oSS << plan[i];
            }
        }
        oSS  << "]";

        std::cout << oSS.str() << std::endl;
    }

    return;
}


void registerPath(const Vector<mcpp::Coordinate2D>& plan,
                  Map<mcpp::Coordinate2D, bool>& occupiedCoords) { 

    const size_t FIRST_SKIP = 0;
    const size_t LAST_SKIP  = 12;

    if (!(plan.getSize() <= FIRST_SKIP + LAST_SKIP)) {

        const size_t end = plan.getSize() - LAST_SKIP;
        
        for (size_t i = FIRST_SKIP; i < end; ++i) {
            occupiedCoords[plan[i]] = true;
        }
    }

    return;
}