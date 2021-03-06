//
// Created by Tobias on 30.12.2015.
//

#include "Folding.hpp"

char Folding::HYDROPHOBIC = '1';

Folding::Folding(shared_ptr<string> chain_ptr)
        : genotype(chain_ptr != 0 ? chain_ptr->length() - 1 : 0),
          cached_fitness(-1),
          protein_chain(chain_ptr)
{
    std::uniform_int_distribution<int> dist(-1, 1);

    for (auto& direction : genotype)
        direction = Grid::Move(dist(Evo::generator));

    if (chain_ptr != 0)
        recalculate_fitness();
}

vector<Grid::Move>::iterator Folding::begin() {
    return genotype.begin();
}

vector<Grid::Move>::iterator Folding::end() {
    return genotype.end();
}

vector<Grid::Move>::const_iterator Folding::cbegin() const {
    return genotype.cbegin();
}

vector<Grid::Move>::const_iterator Folding::cend() const {
    return genotype.cend();
}

uint Folding::size() const {
    return genotype.size();
}

bool Folding::operator==(const Folding& folding) const {
    if (genotype.size() != folding.size())
        return false;

    for (uint i = 0U; i < genotype.size(); ++i)
        if (genotype.at(i) != folding.genotype.at(i))
            return false;

    return true;
}

bool Folding::operator<(const Folding& folding) const {
    return cached_fitness < folding.cached_fitness;
}

uint Folding::hamming_distance(const Folding& folding) const {
    if (genotype.size() != folding.size())
        throw std::runtime_error("Folding::hamming_distance(): Different lengths!");

    uint distance = 0U;
    for (uint i = 0LU; i < genotype.size(); ++i)
        if (genotype.at(i) != folding.genotype.at(i))
            ++distance;

    return distance;
}

void Folding::draw(ostream& out) const {
    Grid grid;
    vector<Point> coords = { grid.position() };

    for (auto& move : genotype) {
        Point coord = grid.move(move);
        coord.x *= 2;
        coord.y *= 2;
        coords.push_back(coord);
    }

    auto xComparer = [](const Point& p1, const Point& p2) { return p1.x < p2.x; };
    auto yComparer = [](const Point& p1, const Point& p2) { return p1.y < p2.y; };

    int xMin = std::min_element(coords.begin(), coords.end(), xComparer)->x;
    int yMin = std::min_element(coords.begin(), coords.end(), yComparer)->y;
    int xDiff = std::max_element(coords.begin(), coords.end(), xComparer)->x - xMin;
    int yDiff = std::max_element(coords.begin(), coords.end(), yComparer)->y - yMin;

    Matrix<char> buffer(xDiff + 1, yDiff + 1, ' ');
    buffer[coords[0].x - xMin][coords[0].y - yMin] = protein_chain->at(0);
    for (uint i(1U); i < coords.size(); ++i) {
        int x = coords.at(i).x - xMin;
        int y = coords.at(i).y - yMin;

        if (buffer[x][y] == ' ')
            buffer[x][y] = protein_chain->at(i);
        else
            buffer[x][y] = 'X';

        int delta_x = coords.at(i).x - coords.at(i - 1).x;
        int delta_y = coords.at(i).y - coords.at(i - 1).y;

        char connector = '-';
        if (delta_y != 0)
            connector = '|';

        buffer[x - delta_x / 2][y - delta_y / 2] = connector;
    }

    for (int y = yDiff; y >= 0; --y) {
        for (int x = 0; x <= xDiff; ++x)
            out << buffer[x][y];
        out << std::endl;
    }
}

ostream& operator<<(ostream& stream, const Folding& folding) {
    stream << "Genotype: ";
    for (uint i = 0U; i < folding.genotype.size(); ++i) {
        switch(folding.genotype.at(i)) {
            case Grid::Move::LEFT:
                stream << 'L';
                break;
            case Grid::Move::STRAIGHT:
                stream << 'S';
                break;
            case Grid::Move::RIGHT:
                stream << 'R';
                break;
            default:
                throw std::runtime_error("operator<<(): unknown movement");
        }
    }

    stream << std::endl
           << "Fitness: " << folding.cached_fitness << std::endl
           << "Conformation: " << std::endl;

    folding.draw(stream);
    return stream;
}

double Folding::fitness() const {
    return cached_fitness;
}

double Folding::recalculate_fitness() {
    uint num_overlaps = 0U;
    uint num_neighbours = 0U;
    Grid grid;
    map<Point, bool> structure = { std::make_pair(grid.position(), protein_chain->at(0) == HYDROPHOBIC) };

    for (uint i(1U); i < protein_chain->length(); ++i) {
        Point pos = grid.move(genotype.at(i - 1));
        bool isHydrophobic = protein_chain->at(i) == HYDROPHOBIC;

        if (structure.find(pos) != structure.end()) {
            structure[pos] = structure[pos] || isHydrophobic;
            ++num_overlaps;
            continue;
        } else {
            structure[pos] = isHydrophobic;
        }

        if (!isHydrophobic)
            continue;

        vector<Point> neighbours = grid.neighbours();
        for (const Point& neighbour : neighbours)
            if (structure.find(neighbour) != structure.end() && structure[neighbour])
                ++num_neighbours;
    }

    cached_fitness = (1.0 + std::pow(num_neighbours, 2)) / (1.0 + std::pow(num_overlaps, 4));

    return cached_fitness;
}