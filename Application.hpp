//
// Created by Tobias on 09.01.2016.
//

#ifndef FOLDING_CONTROL_HPP
#define FOLDING_CONTROL_HPP

#include "algorithm.hpp"
#include "Menu.hpp"

class Application : public Menu {
public:
    enum SelectionStrategy { FitnessProportional, Tournament };

    static Application& get_instance();
    bool is_running();

    void enter();
    void move_down(uint);

    void run();
    void calculate_params();
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
};

// Settings
// Exit

#endif //FOLDING_CONTROL_HPP
