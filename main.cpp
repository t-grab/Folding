#include <iostream>
#include <evo.hpp>
using namespace Evo;

#include "Grid.hpp"

int main() {
    std::cout << "Folding in 2D HP" << std::endl;

    Grid grid;

    grid.move(Grid::LEFT);
    std::cout << grid.position().x << " " << grid.position().y << std::endl;
    grid.move(Grid::STRAIGHT);
    std::cout << grid.position().x << " " << grid.position().y << std::endl;
    grid.move(Grid::RIGHT);
    std::cout << grid.position().x << " " << grid.position().y << std::endl;
    grid.move(Grid::RIGHT);
    std::cout << grid.position().x << " " << grid.position().y << std::endl;

    std::cout << "---" << std::endl;

    std::vector<Point> neighbours(grid.neighbours());
    for (auto& p : neighbours)
        std::cout << p.x << " " << p.y << std::endl;

    return 0;
}