#include "build_path.h"
#include <iostream>



void buildPath(const Vector<mcpp::Coordinate2D>& plan,
               const mcpp::HeightMap& heightMap,
               const mcpp::Chunk& chunk) {

    const int ALLOWED_Y_DIFF = 1;
    mcpp::MinecraftConnection mc{};
    mcpp::BlockType gravelBlock = mcpp::Blocks::GRAVEL;
    
    Vector<mcpp::Coordinate> coords = toHeighestCoords(plan, heightMap);

    // We won't build on the first or last coords
    for (size_t i = 1; i < plan.getSize() - 1; ++i) {
        mcpp::Coordinate curr = coords[i];
        mcpp::Coordinate prev = coords[i - 1];

        //if higher, then cut down
        if (curr.y - prev.y > ALLOWED_Y_DIFF) {
            int newY = prev.y + 1;
            mcpp::Coordinate newCoord(curr.x, newY, curr.z);

            cutDown(curr, newCoord, mc);

            mc.setBlock(newCoord, gravelBlock);

            coords[i] = newCoord;
        }

        //if lower, then build up
        else if (curr.y - prev.y < -ALLOWED_Y_DIFF) {
            int newY = prev.y - 1;
            mcpp::Coordinate newCoord(curr.x, newY, curr.z);

            buildUp(curr, newCoord, mc);
            mc.setBlock(newCoord, gravelBlock);

            coords[i] = newCoord;
        }

        else {
            try {
                mcpp::BlockType currBlock = chunk.get_worldspace(curr);

                if (currBlock == mcpp::Blocks::STILL_WATER ||
                        currBlock == mcpp::Blocks::FLOWING_WATER) {
                    supportGravel(curr, mc);
                }

            } catch(std::out_of_range& excpt) {
                supportGravel(curr, mc); 
            }

            mc.setBlock(curr, gravelBlock);
        }

    }



    return;
}



Vector<mcpp::Coordinate> 
toHeighestCoords(const Vector<mcpp::Coordinate2D>& vec,
                 const mcpp::HeightMap& heightMap) {

    Vector<mcpp::Coordinate> highestCoords{};

    try {

        for (size_t i = 0; i < vec.getSize(); ++i) {
            int height = heightMap.get_worldspace(vec[i]);
            mcpp::Coordinate coord(vec[i].x, height, vec[i].z);
    
            highestCoords.push_back(coord);
        }

    } catch(std::out_of_range& excpt) {
        highestCoords = Vector<mcpp::Coordinate>();
    }

    return highestCoords;
}



void cutDown(const mcpp::Coordinate& prevCoord, 
             const mcpp::Coordinate& newCoord,
             mcpp::MinecraftConnection& mc) {

    if (prevCoord.y > newCoord.y) {

        mcpp::BlockType airBlock = mcpp::Blocks::AIR;
        mcpp::Coordinate initCoord(newCoord);
        ++initCoord.y;
        
        mc.setBlocks(initCoord, prevCoord, airBlock);
        
    }
    
    return;
}



void buildUp(const mcpp::Coordinate& prevCoord, 
             const mcpp::Coordinate& newCoord,
             mcpp::MinecraftConnection& mc) {

    if (prevCoord.y < newCoord.y) {

        mcpp::BlockType grassBlock = mcpp::Blocks::DIRT;
        mcpp::Coordinate initCoord(newCoord);
        --initCoord.y;
        
        mc.setBlocks(initCoord, prevCoord, grassBlock);
    }

    return;
}


void supportGravel(const mcpp::Coordinate& currCoord,
                   mcpp::MinecraftConnection& mc) {
    
    mcpp::Coordinate coord(currCoord);
    
    coord.y--;
    mc.setBlock(coord, mcpp::Blocks::DIRT);
    
    return;
}


