//
// Created by Tobias on 09.01.2016.
//

#include "Diversity.hpp"

string Diversity::sep = "|";

Diversity::Diversity(double m, double s) : mean(m), sd(s) {}

void Diversity::calculate(const vector<Folding>& solutions) {
    if (solutions.size() < 2U)
        throw runtime_error("Diversity::calculate(): less than 2 elements");

    uint size = solutions.size();

    double mn = 0.0;
    double n = 0.0;
    double m = 0.0;
    double s = 0.0;
    double m_prev = 0.0;

    for (uint i = 0U; i < size; ++i) {
        for (uint j = i + 1; j < size; ++j) {
            double distance = solutions.at(i).hamming_distance(solutions.at(j));

            mn += distance;

            ++n;
            m_prev = m;
            m += (distance - m_prev) / n;
            s += (distance - m_prev) * (distance - m);
        }
    }

    mean = mn / n;
    sd = std::sqrt(s / (n - 1));
}

ostream& operator<<(ostream& stream, Diversity div) {
    stream << div.mean << div.sep << div.sd;
    return stream;
}
