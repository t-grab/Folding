//
// Created by Tobias on 07.01.2016.
//

#ifndef FOLDING_ALGORITHM_HPP
#define FOLDING_ALGORITHM_HPP

#include <iostream>
#include <stdexcept>
#include <ctime>
#include <evo.hpp>
using namespace Evo;

#include "Folding.hpp"
#include "Diversity.hpp"
#include "FoldingParams.hpp"

vector<Folding> init(uint population_size, shared_ptr<string> protein);

struct Fitness {
    double operator()(const Folding& folding);
};

Grid::Move replace(const Folding& folding, vector<Grid::Move>::iterator iter);

void add_generation_result(Result<Folding, double, Diversity>& result, const vector<Folding>& solutions, uint pop_size, bool measure_diversity);
void add_generation_result(Result<FoldingParams, double, double>& result, const vector<FoldingParams>& solutions, uint pop_size);

template<typename T>
double accumulate_fitness(double acc, const T& individual) {
    return acc + individual.fitness();
}

template<typename Select>
Result<Folding, double, Diversity> solve(shared_ptr<string> protein, uint pop_size, uint max_gen, Select select,
                                         bool measure_diversity, double c_rate, double m_rate, double max_runtime)
{
    std::clock_t start_time = std::clock();
    double run_time = 0.0;
    double avg_loop_time = 0.0;

    vector<Folding> population = init(pop_size, protein);
    Result<Folding, double, Diversity> result;
    add_generation_result(result, population, pop_size, measure_diversity);

    max_runtime = max_runtime - static_cast<double>(std::clock() - start_time) / CLOCKS_PER_SEC;
    for (uint gen = 0U; gen < max_gen; ++gen) {
        start_time = std::clock();

        std::vector<Folding> parents(pop_size);
        std::uniform_real_distribution<double> uniform;
        std::exponential_distribution<double> exponential(1 / (m_rate * (protein->length() - 1)));
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

        run_time = static_cast<double>(std::clock() - start_time) / CLOCKS_PER_SEC;
        max_runtime = max_runtime - run_time;
        avg_loop_time = (gen * avg_loop_time + run_time) / (gen + 1);
        if (avg_loop_time > max_runtime)
            break;
    }

    return result;
}

template<typename Select>
Result<FoldingParams, double, double> solve_params(shared_ptr<string> protein, uint pop_size, uint pop_size_inner, uint max_gen, uint max_gen_inner,
                                                   Select select, uint runs, double c_rate, double m_rate, double max_runtime = std::numeric_limits<double>::max())
{
    vector<FoldingParams> population;
    for (uint i = 0U; i < pop_size; ++i)
        population.push_back(FoldingParams(protein, pop_size_inner, max_gen_inner, select, runs, max_runtime));

    Result<FoldingParams, double, double> result;
    add_generation_result(result, population, pop_size);

    for (uint gen = 0U; gen < max_gen; ++gen) {
        std::cout << "Starting generation " << gen + 1 << std::endl;

        vector<FoldingParams> parents(pop_size);
        std::uniform_real_distribution<double> uniform;

        select::tournament(population.begin(), population.end(), parents.begin(), pop_size, 20, [](const FoldingParams& params) { return params.fitness(); });

        for (uint i = 0U; i < pop_size; ++i) {
            if (i % 2 == 0 && i < (pop_size - 1) && uniform(generator) <= c_rate) {
                std::tuple<FoldingParams, FoldingParams> children = recombine::one_point_crossover(parents.at(i), parents.at(i + 1));
                parents.at(i) = std::get<0>(children);
                parents.at(i + 1) = std::get<0>(children);
            }

            parents.at(i) = mutate::creep(parents.at(i), m_rate, 0.0, 1.0);
            parents.at(i).recalculate_fitness(protein, pop_size_inner, max_gen_inner, select, runs, max_runtime);
        }

        population.clear();
        population = parents;

        add_generation_result(result, population, pop_size);
    }

    return result;
}

#endif //FOLDING_ALGORITHM_HPP
