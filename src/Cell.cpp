#include "Cell.h"


Cell::Cell(mcpp::Coordinate2D coord, int f, int h)
    : coord(coord), f(f), h(h)
{}


bool Cell::operator<(const Cell& other) const {

    bool isLess = false;
    
    if (f == other.f) {
        isLess = h < other.h;
    }
    else {
        isLess = f < other.f;
    }

    return isLess;
}

bool Cell::operator>(const Cell& other) const {
    return other < *this;
}



Cell Cell::neighborIn(Direction direction) const {

    Cell newCell;

    switch(direction) {
        case Direction::North :
            newCell.coord = coord;
            --newCell.coord.z;

            break;

        case Direction::South :
            newCell.coord = coord;
            ++newCell.coord.z;

            break;

        case Direction::West :
            newCell.coord = coord;
            --newCell.coord.x;

            break;

        case Direction::East :
            newCell.coord = coord;
            ++newCell.coord.x;

            break;

        default:
            newCell.coord = coord;

            break;
    }

    return newCell;
}


Vector<Cell> Cell::getNeighbors() const {
    Vector<Cell> result;

    result.push_back(neighborIn(Direction::North));
    result.push_back(neighborIn(Direction::South));
    result.push_back(neighborIn(Direction::East));
    result.push_back(neighborIn(Direction::West));

    return result;
}