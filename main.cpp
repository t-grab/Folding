#include <iostream>
#include <stdexcept>
#include <evo.hpp>
using namespace Evo;

#include "Folding.hpp"

vector<Folding> init(uint population_size, shared_ptr<string> protein) {
    vector<Folding> population;
    for (uint i(0U); i < population_size; ++i)
        population.push_back(Folding(protein));
    return population;
}

double fitness_func(const Folding& folding) {
    return folding.fitness();
}

Grid::Move random_func(size_t idx, Grid::Move move) {
    std::uniform_int_distribution<size_t> dist(1, 2);
    return Grid::Move((dist(generator) + (move + 1)) % 3 - 1);
}

Result<Folding, double> solve
        (
                shared_ptr<string> protein,
                uint population_size,
                uint max_generation,
                double c_rate,
                double m_rate
        )
{
    vector<Folding> population = init(population_size, protein);
    Result<Folding, double> result;

    for (uint gen = 0U; gen < max_generation; ++gen) {
        std::vector<Folding> parents;
        std::uniform_real_distribution<double> dist;
        bool recombined = false;

        parents.resize(population_size);
        select::fitness_proportional(population.begin(), population.end(), parents.begin(), population_size, fitness_func, select::Roulette_Wheel<double>());

        for (uint i = 0U; i < population_size; ++i) {
            if (i % 2 == 0) {
                recombined = false;

                if (i < (population_size - 1) && dist(generator) <= c_rate) {
                    std::tuple<Folding, Folding> children = recombine::one_point_crossover(parents.at(i), parents.at(i + 1));
                    parents.at(i) = std::get<0>(children);
                    parents.at(i + 1) = std::get<1>(children);

                    recombined = true;
                }
            }

            uint changes = 0U;
            parents.at(i) = mutate::point(parents.at(i), [&]() {
                bool change = dist(generator) <= m_rate;
                if (change) ++changes;
                return change;
            }, random_func);

            if (changes > 0 || recombined)
                parents.at(i).recalculate_fitness();
        }

        double max_fitness = std::max_element(parents.begin(), parents.end(),
                                              [](const Folding& one, const Folding& two) {
                                                  return one.fitness() < two.fitness();
                                              })->fitness();

        double avg_fitness = std::accumulate(parents.begin(), parents.end(), 0.0,
                                             [](double acc, const Folding& folding) {
                                                 return acc + folding.fitness();
                                             });
        avg_fitness /= population_size;

        vector<Folding> best;
        for (auto& folding : parents)
            if (folding.fitness() == max_fitness)
                best.push_back(folding);

        result.add_generation(max_fitness, avg_fitness, best.begin(), best.end());

        population.clear();
        population = parents;
    }

    return result;
}

const string SEQ20 = "10100110100101100101";
const string SEQ24 = "110010010010010010010011";
const string SEQ25 = "0010011000011000011000011";
const string SEQ36 = "000110011000001111111001100001100100";
const string SEQ48 = "001001100110000011111111110000001100110010011111";
const string SEQ50 = "11010101011110100010001000010001000101111010101011";

int main() {
    try {
        string chain = SEQ20;

        std::cout << "Folding in 2D HP" << std::endl
                  << "Used chain: " << chain << std::endl;

        shared_ptr<string> protein = std::make_shared<string>(chain);

        auto result = solve(protein, 400, 100, 0.3, 0.1);
        result.print();

        for (auto solution : result.fittest()) {
            solution.recalculate_fitness();
            std::cout << "Fitness: " << solution.fitness() << std::endl
                      << solution;
        }

        return 0;
    }
    catch (std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return -3;
    }
    catch (std::out_of_range& e) {
        std::cerr << "Out of range error: " << e.what() << std::endl;
        return -2;
    }
    catch (...) {
        std::cerr << "Unknown error!" << std::endl;
        return -1;
    }
}