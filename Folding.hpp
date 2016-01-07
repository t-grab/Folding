//
// Created by Tobias on 30.12.2015.
//

#ifndef FOLDING_FOLDING_HPP
#define FOLDING_FOLDING_HPP

#include <array>
using std::array;

#include <iostream>
using std::ostream;

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include <map>
using std::map;

#include <evo.hpp>
using std::vector;

#include <limits>
using std::numeric_limits;

#include "Grid.hpp"
#include "Matrix.hpp"

class Folding {
public:
    explicit Folding(shared_ptr<string> = 0);

    typedef vector<Grid::Move>::iterator iterator;
    typedef vector<Grid::Move>::const_iterator const_iterator;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    uint size() const;
    bool operator==(const Folding&) const;
    bool operator<(const Folding&) const;
    uint hamming_distance(const Folding&) const;

    double fitness() const;
    double recalculate_fitness();

    friend ostream& operator<<(ostream&, const Folding&);
    void draw(ostream& = std::cout) const;

private:
    vector<Grid::Move> genotype;
    double cached_fitness;
    shared_ptr<string> protein_chain;

    static char HYDROPHOBIC;
};

namespace std {
    template<>
    struct hash<Folding> {
        size_t operator()(const Folding& fold) const noexcept {
            const unsigned long limit = numeric_limits<size_t>::max();
            unsigned long hash = 0LU;

            for (auto gene_itr = fold.cbegin(); gene_itr != fold.cend(); std::advance(gene_itr, 1))
                hash = (hash * 31 + *gene_itr) % limit;

            return static_cast<size_t>(hash);
        }
    };
}

#endif //FOLDING_FOLDING_HPP
