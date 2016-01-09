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
#include "Diversity.hpp"

vector<Folding> init(uint population_size, shared_ptr<string> protein);

struct Fitness {
    double operator()(const Folding& folding);
};

Grid::Move replace(const Folding& folding, vector<Grid::Move>::iterator iter);

void add_generation_result(Result<Folding, double, Diversity>& result, const vector<Folding>& solutions, uint pop_size, bool measure_diversity);
double accumulate_fitness(double acc, const Folding& folding);

template<typename Select>
Result<Folding, double, Diversity> solve(shared_ptr<string> protein, uint pop_size, uint max_gen, Select select, bool measure_diversity, double c_rate, double m_rate) {
    vector<Folding> population = init(pop_size, protein);
    Result<Folding, double, Diversity> result;

    add_generation_result(result, population, pop_size, measure_diversity);

    for (uint gen = 0U; gen < max_gen; ++gen) {
        std::vector<Folding> parents(pop_size);
        std::uniform_real_distribution<double> uniform;
        std::exponential_distribution<double> exponential(1 / (c_rate * protein->length() - 1));
        bool recombined = false;

        select(population.begin(), population.end(), parents.begin());

        for (uint i = 0U; i < pop_size; ++i) {
            if (i % 2 == 0) {
                recombined = false;

                if (i < (pop_size - 1) && uniform(generator) <= c_rate) {
                    std::tuple<Folding, Folding> children = recombine::one_point_crossover(parents.at(i), parents.at(i + 1));
                    parents.at(i) = std::get<0>(children);
                    parents.at(i + 1) = std::get<1>(children);

                    recombined = true;
                }
            }

            uint mutations = static_cast<uint>(round(exponential(generator)));
            parents.at(i) = mutate::point_draw(parents.at(i), mutations, replace);

            if (mutations > 0 || recombined)
                parents.at(i).recalculate_fitness();
        }

        population.clear();
        population = parents;

        add_generation_result(result, population, pop_size, measure_diversity);
    }

    return result;
}

#endif //FOLDING_ALGORITHM_HPP
