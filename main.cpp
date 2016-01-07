#include "algorithm.hpp"

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

        auto result = solve(protein, 200, 100, 0.2, 0.03);
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