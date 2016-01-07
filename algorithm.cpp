//
// Created by Tobias on 07.01.2016.
//

#include "algorithm.hpp"

Result<Folding, double, double> solve(shared_ptr<string> protein, uint pop_size, uint max_gen, double c_rate, double m_rate) {
    vector<Folding> population = init(pop_size, protein);
    Result<Folding, double, double> result;

    for (uint gen = 0U; gen < max_gen; ++gen) {
        std::vector<Folding> parents(pop_size);
        std::uniform_real_distribution<double> uniform;
        std::exponential_distribution<double> exponential(1 / (c_rate * protein->length() - 1));
        bool recombined = false;

        select::fitness_proportional(population.begin(), population.end(), parents.begin(), pop_size, fitness, select::Roulette_Wheel<double>());

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

        double max_fitness = std::max_element(parents.begin(), parents.end())->fitness();
        double avg_fitness = std::accumulate(parents.begin(), parents.end(), 0.0, accumulate_fitness) / pop_size;

        vector<Folding> best;
        for (auto& folding : parents)
            if (folding.fitness() == max_fitness)
                best.push_back(folding);

        result.add_generation(max_fitness, avg_fitness, calc_avg_distance(best), best.begin(), best.end());

        population.clear();
        population = parents;
    }

    return result;
}

vector<Folding> init(uint population_size, shared_ptr<string> protein) {
    vector<Folding> population;
    for (uint i(0U); i < population_size; ++i)
        population.push_back(Folding(protein));
    return population;
}

double fitness(const Folding& folding) {
    return folding.fitness();
}

Grid::Move replace(size_t idx, Grid::Move move) {
    std::uniform_int_distribution<size_t> dist(1, 2);
    return Grid::Move((dist(generator) + (move + 1)) % 3 - 1);
}

double accumulate_fitness(double acc, const Folding& folding) {
    return acc + folding.fitness();
}

double calc_avg_distance(const vector<Folding>& solutions) {
    if (solutions.size() < 2U)
        return 0.0;

    uint size = solutions.size();
    double sum = 0.0;

    for (uint i = 0U; i < size; ++i)
        for (uint j = i + 1; j < size; ++j)
            sum += solutions.at(i).hamming_distance(solutions.at(j));

    return sum / (size * (size - 1) / 2);
}