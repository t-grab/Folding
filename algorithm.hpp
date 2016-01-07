//
// Created by Tobias on 07.01.2016.
//

#ifndef FOLDING_ALGORITHM_HPP
#define FOLDING_ALGORITHM_HPP

#include <iostream>
#include <stdexcept>
#include <evo.hpp>
using namespace Evo;

#include "Folding.hpp"

Result<Folding, double> solve(shared_ptr<string> protein, uint pop_size, uint max_gen, double c_rate, double m_rate);

vector<Folding> init(uint population_size, shared_ptr<string> protein);
double fitness(const Folding& folding);
Grid::Move replace(size_t idx, Grid::Move move);
double accumulate_fitness(double acc, const Folding& folding);

#endif //FOLDING_ALGORITHM_HPP
