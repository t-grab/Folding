//
// Created by Tobias on 30.12.2015.
//

#ifndef FOLDING_GRID_HPP
#define FOLDING_GRID_HPP

#include <vector>

int inline mod(int, int);

struct Point {
    Point();
    Point(int, int);

    bool operator<(const Point& p) const;

    int x;
    int y;
};

class Grid {
public:
    enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };
    enum Move { STRAIGHT = 0, LEFT = -1, RIGHT = 1 };

    Grid();

    Point position() const;
    Direction direction() const;

    Point move(Move);
    std::vector<Point> neighbours() const;
private:
    Point pos;
    Direction dir;

    static std::vector<Point> offsetVectors;
};

#endif //FOLDING_POSITION_HPP
