//
// Created by Tobias on 04.01.2016.
//

#ifndef FOLDING_MATRIX_HPP
#define FOLDING_MATRIX_HPP

#include <vector>
using std::vector;

typedef unsigned int uint;

template<typename T>
class Matrix {
public:
    Matrix(uint n_rows, uint n_cols, T std = T()) : data(n_rows) {
        for (auto& vec : data)
            vec.resize(n_cols, std);
    }

    vector<T>& operator[](uint row) {
        return data.at(row);
    }
private:
    vector<vector<T>> data;
};

#endif //FOLDING_MATRIX_HPP
