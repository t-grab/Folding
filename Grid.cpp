//
// Created by Tobias on 30.12.2015.
//

#include "Grid.hpp"

int inline mod(int a, int b) {
    return (a % b + b) % b;
}


Point::Point() : x(0), y(0) {}

Point::Point(int ix, int iy) : x(ix), y(iy) {}


std::vector<Point> Grid::offsetVectors = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

Grid::Grid() : pos(0, 0), dir(Grid::Direction::EAST) {}

Point Grid::position() const {
    return pos;
}

Grid::Direction Grid::direction() const {
    return dir;
}

Point Grid::move(Grid::Move move) {
    dir = Grid::Direction(mod(dir + move, 4));

    pos.x += offsetVectors[dir].x;
    pos.y += offsetVectors[dir].y;
}

std::vector<Point> Grid::neighbours() const {
    Direction skip = Grid::Direction(mod(dir + 2, 4));
    std::vector<Point> neighbours;

    for (unsigned int i(0U); i < offsetVectors.size(); ++i)
        if (i != skip)
            neighbours.push_back(Point(pos.x + offsetVectors[i].x, pos.y + offsetVectors[i].y));

    return neighbours;
}