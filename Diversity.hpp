//
// Created by Tobias on 09.01.2016.
//

#ifndef FOLDING_DIVERSITY_HPP
#define FOLDING_DIVERSITY_HPP

#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;
#include <iostream>
using std::ostream;

#include "Folding.hpp"

class Diversity {
public:
    Diversity(double m = 0.0, double s = 0.0);

    void calculate(const vector<Folding>& solutions);

    friend ostream& operator<<(ostream& stream, Diversity div);
private:
    double mean;
    double sd;

    static string sep;
};

#endif //FOLDING_DIVERSITY_HPP
