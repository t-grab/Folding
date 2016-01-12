//
// Created by Tobias on 12.01.2016.
//

#ifndef FOLDING_FOLDINGPARAMS_HPP
#define FOLDING_FOLDINGPARAMS_HPP

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>

#include <evo.hpp>
using namespace Evo;

#include "Folding.hpp"
#include "Diversity.hpp"

template<typename Select>
Result<Folding, double, Diversity> solve(shared_ptr<string> protein, uint pop_size, uint max_gen, Select select,
                                         bool measure_diversity, double c_rate, double m_rate, double max_runtime);

typedef unsigned int uint;

class FoldingParams {
public:
    FoldingParams();

    template<typename Select>
    FoldingParams(shared_ptr<string> protein, uint pop_size, uint max_gens, Select select, uint runs, double runtime = std::numeric_limits<double>::max())
            : params(2), cached_fitness(-1.0)
    {
        std::uniform_real_distribution<double> uniform;
        for (auto& param : params)
            param = uniform(generator);

        recalculate_fitness(protein, pop_size, max_gens, select, runs, runtime);
    }

    typedef vector<double>::iterator iterator;
    typedef vector<double>::const_iterator const_iterator;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool operator==(const FoldingParams&) const;
    bool operator<(const FoldingParams&) const;

    uint size() const;

    double fitness() const;

    template<typename Select>
    void recalculate_fitness(shared_ptr<string> protein, uint pop_size, uint max_gens, Select select, uint runs, double max_runtime = std::numeric_limits<double>::max()) {
        cached_fitness = 0.0;
        for (uint i = 0U; i < runs; ++i) {
            auto result = solve(protein, pop_size, max_gens, select, false, params[0], params[1], max_runtime);
            cached_fitness += result.max() * result.fittest().size();
        }
        cached_fitness /= runs;
    }

    friend ostream& operator<<(ostream&, const FoldingParams&);
private:
    vector<double> params;
    double cached_fitness;
};

namespace std {
    template<>
    struct hash<FoldingParams> {
        size_t operator()(const FoldingParams& params) const noexcept {
            const unsigned long limit = numeric_limits<size_t>::max();
            unsigned long hash = 0LU;

            for (auto gene_itr = params.cbegin(); gene_itr != params.cend(); std::advance(gene_itr, 1))
                hash = static_cast<unsigned long>(round(hash * 997 + *gene_itr)) % limit;

            return static_cast<size_t>(hash);
        }
    };
}

#endif //FOLDING_FOLDINGPARAMS_HPP
