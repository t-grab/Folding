//
// Created by Tobias on 30.12.2015.
//

#ifndef FOLDING_FOLDING_HPP
#define FOLDING_FOLDING_HPP

#include <string>
using std::vector;
#include <evo.hpp>

#include "Position.hpp"

class Folding {
public:
    Folding(uint);

    vector<Position::Direction>::iterator begin();
    vector<Position::Direction>::iterator end();

    void draw() const;
private:
    vector<Position::Direction> genotype;
};

#endif //FOLDING_FOLDING_HPP
