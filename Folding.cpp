//
// Created by Tobias on 30.12.2015.
//

#include "Folding.hpp"

Folding::Folding(uint n) : genotype(n) {
    std::uniform_int_distribution dist(0, 2);

    for (auto& direction : genotype)
        direction = Position::Direction(dist(Evo::generator));
}

vector<Position::Direction>::iterator Folding::begin() {
    return genotype.begin();
}

vector<Position::Direction>::iterator Folding::end() {
    return genotype.end();
}

void Folding::draw() const {

}