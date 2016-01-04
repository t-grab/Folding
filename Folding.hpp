//
// Created by Tobias on 30.12.2015.
//

#ifndef FOLDING_FOLDING_HPP
#define FOLDING_FOLDING_HPP

#include <array>
using std::array;
#include <iostream>
using std::ostream;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
#include <map>
using std::map;
#include <evo.hpp>
using std::vector;

#include "Grid.hpp"
#include "Matrix.hpp"

class Folding {
public:
    Folding(shared_ptr<string>);

    vector<Grid::Move>::iterator begin();
    vector<Grid::Move>::iterator end();

    double fitness();
    double recalculate_fitness();

    void draw(ostream& = std::cout) const;

private:
    vector<Grid::Move> genotype;
    double cached_fitness;
    shared_ptr<string> protein_chain;

    static char HYDROPHOBIC;
};

#endif //FOLDING_FOLDING_HPP
