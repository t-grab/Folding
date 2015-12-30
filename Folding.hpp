//
// Created by Tobias on 30.12.2015.
//

#ifndef FOLDING_FOLDING_HPP
#define FOLDING_FOLDING_HPP

#include <evo.hpp>
using std::vector;

#include "Grid.hpp"

class Folding {
public:
    Folding(uint);

    vector<Grid::Move>::iterator begin();
    vector<Grid::Move>::iterator end();

    void draw() const;
private:
    vector<Grid::Move> genotype;
};

#endif //FOLDING_FOLDING_HPP
