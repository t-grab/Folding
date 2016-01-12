//
// Created by Tobias on 07.01.2016.
//

#include "algorithm.hpp"

vector<Folding> init(uint population_size, shared_ptr<string> protein) {
    vector<Folding> population;
    for (uint i(0U); i < population_size; ++i)
        population.push_back(Folding(protein));
    return population;
}

double Fitness::operator()(const Folding& folding) {
    return folding.fitness();
}

Grid::Move replace(const Folding& folding, vector<Grid::Move>::iterator iter) {
    std::uniform_int_distribution<size_t> dist(1, 2);
    return Grid::Move((dist(generator) + (*iter + 1)) % 3 - 1);
}

void add_generation_result(Result<Folding, double, Diversity>& result, const vector<Folding>& solutions, uint pop_size, bool measure_diversity) {
    double max_fitness = std::max_element(solutions.begin(), solutions.end())->fitness();
    double avg_fitness = std::accumulate(solutions.begin(), solutions.end(), 0.0, accumulate_fitness<Folding>) / pop_size;

    vector<Folding> best;
    for (auto& folding : solutions)
        if (folding.fitness() == max_fitness)
            best.push_back(folding);

    if (measure_diversity) {
        Diversity diversity;
        diversity.calculate(solutions);

        result.add_generation(max_fitness, avg_fitness, diversity, best.begin(), best.end());
    } else {
        result.add_generation(max_fitness, avg_fitness, best.begin(), best.end());
    }
}

void add_generation_result(Result<FoldingParams, double, double>& result, const vector<FoldingParams>& solutions, uint pop_size) {
    double max_fitness = std::max_element(solutions.begin(), solutions.end())->fitness();
    double avg_fitness = std::accumulate(solutions.begin(), solutions.end(), 0.0, accumulate_fitness<FoldingParams>) / pop_size;

    vector<FoldingParams> best;
    for (auto& params : solutions)
        if (params.fitness() == max_fitness)
            best.push_back(params);

    result.add_generation(max_fitness, avg_fitness, best.begin(), best.end());
}