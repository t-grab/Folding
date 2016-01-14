//
// Created by Tobias on 09.01.2016.
//

#include "Application.hpp"

Application::Application() : Menu("Foldings for the 2D-HP-model", 0),
    running(true), protein("10100110100101100101"), population_size(200), max_generations(100), selection(Tournament),
    tournament_size(20), crossover_rate(0.2), mutation_rate(0.05), measure_diversity(false), verbose_output(true),
    max_runtime(60.0), rscript(), param_runs(10), param_population_size(50), param_max_generations(30),
    param_crossover_rate(0.2), param_mutation_rate(0.2)
{
    auto run_func = [&]() { run(); };
    auto calc_params_func = [&]() { calculate_params(); };
    auto load_config_func = [&]() { load_config(); };

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
            settings->add_submenu(new Parameter<uint>("Parameter Tuning: Population size", settings, param_population_size));
            settings->add_submenu(new Parameter<uint>("Parameter Tuning: Max generations", settings, param_max_generations));
            settings->add_submenu(new Parameter<double>("Parameter Tuning: Crossover rate", settings, param_crossover_rate));
            settings->add_submenu(new Parameter<double>("Parameter Tuning: Mutation rate", settings, param_mutation_rate));
    add_submenu(settings);
    add_submenu(make_action("Reload config file", this, load_config_func));

    load_config();
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

void Application::execute(uint idx) {
    if (is_active() && idx == get_submenus().size() + 1)
        running = false;
    else
        Menu::execute(idx);
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

    result.print(cout, "\t", verbose_output);

    uint time = std::clock();

    sstream log_file_name;
    log_file_name << "analysis_" << time << ".csv";
    sstream pdf_file_name;
    pdf_file_name << "evolution_" << time << ".pdf";

    ofstream file;
    file.open(log_file_name.str());
    result.print(file, ",", false);
    file.close();

    sstream command;
    command << "cmd /c " << rscript << " run_analysis.R " << log_file_name.str() << " " << pdf_file_name.str();
    system(command.str().c_str());
}

void Application::calculate_params() {
    std::shared_ptr<string> ptr = std::make_shared<string>(protein);
    Result<FoldingParams, double, double> result;

    switch(selection) {
        case FitnessProportional: {
            selector::fitness_proportional<Fitness, select::SUS<double>> proportional(population_size, Fitness(), select::SUS<double>(population_size));
            result = solve_params(ptr, param_population_size, population_size, param_max_generations, max_generations,
                                  proportional, param_runs, param_crossover_rate, param_mutation_rate);
            break;
        }
        case Tournament: {
            selector::tournament<Fitness> tournament(population_size, tournament_size, Fitness());
            result = solve_params(ptr, param_population_size, population_size, param_max_generations, max_generations,
                                  tournament, param_runs, param_crossover_rate, param_mutation_rate);
            break;
        }
        default:
            throw runtime_error("Undefined selection mechanism");
    }

    result.print(cout, "\t", true);
}

void Application::load_config() {
    ifstream config;
    config.open("config");

    if (!config)
        throw runtime_error("Application::load_config: config file could not be opened");

    read<string>(config);
    protein = read<string>(config);

    read<string>(config);
    population_size = read<uint>(config);

    read<string>(config);
    max_generations = read<uint>(config);

    read<string>(config);
    char sel = read<char>(config);
    switch(sel) {
        case 'T':
            selection = Tournament;
            break;
        case 'F':
            selection = FitnessProportional;
            break;
        default:
            throw runtime_error("Application::load_config(): unknown selection strategy read from config file");
    }

    read<string>(config);
    tournament_size = read<uint>(config);

    read<string>(config);
    crossover_rate = read<double>(config);

    read<string>(config);
    mutation_rate = read<double>(config);

    read<string>(config);
    measure_diversity = read<bool>(config);

    read<string>(config);
    verbose_output = read<bool>(config);

    read<string>(config);
    max_runtime = read<double>(config);

    read<string>(config);
    rscript = read<string>(config);

    read<string>(config);
    param_runs = read<uint>(config);

    read<string>(config);
    param_population_size = read<uint>(config);

    read<string>(config);
    param_max_generations = read<uint>(config);

    read<string>(config);
    param_crossover_rate = read<double>(config);

    read<string>(config);
    param_mutation_rate = read<double>(config);

    config.close();
}