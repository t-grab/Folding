//
// Created by Tobias on 30.12.2015.
//

#include "Folding.hpp"

Folding::Folding(uint n) : genotype(n) {
    std::uniform_int_distribution<int> dist(0, 2);

    for (auto& direction : genotype)
        direction = Grid::Move(dist(Evo::generator));
}

vector<Grid::Move>::iterator Folding::begin() {
    return genotype.begin();
}

vector<Grid::Move>::iterator Folding::end() {
    return genotype.end();
}

void Folding::draw() const {

}