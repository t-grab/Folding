//
// Created by Tobias on 12.01.2016.
//

#include "FoldingParams.hpp"

FoldingParams::FoldingParams() : params(), cached_fitness(-1) {}

FoldingParams::iterator FoldingParams::begin() {
    return params.begin();
}

FoldingParams::iterator FoldingParams::end() {
    return params.end();
}

FoldingParams::const_iterator FoldingParams::cbegin() const {
    return params.cbegin();
}

FoldingParams::const_iterator FoldingParams::cend() const {
    return params.cend();
}

bool FoldingParams::operator==(const FoldingParams& param) const {
    if (params.size() != param.size())
        return false;

    for (uint i = 0U; i < params.size(); ++i)
        if (params.at(i) != param.params.at(i))
            return false;

    return true;
}

bool FoldingParams::operator<(const FoldingParams& param) const {
    return cached_fitness < param.cached_fitness;
}

uint FoldingParams::size() const {
    return params.size();
}

double FoldingParams::fitness() const {
    return cached_fitness;
}

ostream& operator<<(ostream& stream, const FoldingParams& params) {
    stream << "Crossover rate: " << params.params[0] << ", Mutation rate " << params.params[1] << std::endl;
}