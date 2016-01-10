//
// Created by Tobias on 09.01.2016.
//

#include "Application.hpp"

Application::Application() : Menu("Foldings for the 2D-HP-model", 0),
    running(true), protein("10100110100101100101"), population_size(200), max_generations(100), selection(Tournament),
    tournament_size(20), crossover_rate(0.2), mutation_rate(0.05), measure_diversity(false), verbose_output(true), max_runtime(60.0)
{
    auto run_func = [&]() { run(); };
    auto calc_params_func = [&]() { calculate_params(); };

    add_submenu(make_action("Run", this, run_func));
    add_submenu(make_action("Calculate Parameters", this, calc_params_func));
        BidirectionalMenu* settings = new BidirectionalMenu("Settings", this);
            settings->add_submenu(new Parameter<string>("Protein chain", settings, protein));
            settings->add_submenu(new Parameter<uint>("Population size", settings, population_size));
            settings->add_submenu(new Parameter<uint>("Max generations", settings, max_generations));
                BidirectionalMenu* selection = new BidirectionalMenu("Selection strategy", settings);
                    selection->add_submenu(new Action<void(*)()>("Fitness Proportional", selection, 0));
                    selection->add_submenu(new Action<void(*)()>("Tournament", selection, 0));
            settings->add_submenu(selection);
            settings->add_submenu(new Parameter<uint>("Tournament size", settings, tournament_size));
            settings->add_submenu(new Parameter<double>("Crossover rate", settings, crossover_rate));
            settings->add_submenu(new Parameter<double>("Mutation rate", settings, mutation_rate));
            settings->add_submenu(new Parameter<bool>("Measure Diversity", settings, measure_diversity));
            settings->add_submenu(new Parameter<bool>("Verbose output", settings, verbose_output));
            settings->add_submenu(new Parameter<double>("Maximum running time", settings, max_runtime));
    add_submenu(settings);
}

Application& Application::get_instance() {
    static Application app;
    return app;
}

bool Application::is_running() {
    return running;
}

void Application::enter() {
    Menu::enter();
    print_option(get_submenus().size() + 1, "Exit");
}

void Application::move_down(uint idx) {
    if (is_active() && idx == get_submenus().size() + 1)
        running = false;
    else
        Menu::move_down(idx);
}

void Application::run() {
    Result<Folding, double, Diversity> result;
    std::shared_ptr<string> ptr = std::make_shared<string>(protein);

    switch(selection) {
        case FitnessProportional: {
            selector::fitness_proportional<Fitness, select::SUS<double>> proportional(population_size, Fitness(), select::SUS<double>(population_size));
            result = solve(ptr, population_size, max_generations, proportional, measure_diversity, crossover_rate, mutation_rate, max_runtime);
            break;
        }
        case Tournament: {
            selector::tournament<Fitness> tournament(population_size, tournament_size, Fitness());
            result = solve(ptr, population_size, max_generations, tournament, measure_diversity, crossover_rate, mutation_rate, max_runtime);
            break;
        }
        default:
            throw runtime_error("Undefined selection mechanism");
    }

    result.print();

    if (verbose_output)
        for (auto solution : result.fittest())
            std::cout << "Fitness: " << solution.fitness() << std::endl << solution;
}

void Application::calculate_params() {

}