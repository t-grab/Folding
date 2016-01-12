//
// Created by Tobias on 09.01.2016.
//

#ifndef FOLDING_CONTROL_HPP
#define FOLDING_CONTROL_HPP

#include <iostream>
#include <fstream>
using std::ifstream;
using std::ofstream;

#include "algorithm.hpp"
#include "Menu.hpp"

class Application : public Menu {
public:
    enum SelectionStrategy { FitnessProportional, Tournament };

    static Application& get_instance();
    bool is_running();

    void enter();
    void execute(uint);

    void run();
    void calculate_params();

    void load_config();
private:
    Application();

    bool running;

    string protein;
    uint population_size;
    uint max_generations;
    SelectionStrategy selection;
    uint tournament_size;
    double crossover_rate;
    double mutation_rate;
    bool measure_diversity;
    bool verbose_output;
    double max_runtime;
    string rscript;

    uint param_runs;
    uint param_population_size;
    uint param_max_generations;
    double param_crossover_rate;
    double param_mutation_rate;
};

template<typename T>
T read(ifstream& stream) {
    T value;
    stream >> value;

    if (!stream)
        throw runtime_error("read(): Could not read value from stream");

    return value;
}

#endif //FOLDING_CONTROL_HPP
