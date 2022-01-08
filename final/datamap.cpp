#include "datamap.hpp"
#include <iostream>

Datamap::Datamap(int n) {
    this->dimension = n;
    this->dataMatrix = this->initDM(n);
}

std::vector<std::vector<double>> Datamap::initDM(int n) {
    std::vector<std::vector<double>> out;
    out.reserve(n);
    
    for (int row = 0; row < n; row++) {
        std::vector<double> currRow;
        currRow.reserve(n);
        for (int col = 0; col < n; col++) {
            currRow.push_back(0.0);
        }
        out.push_back(currRow);
    }

    return out;
}

double Datamap::getData(int x, int y) {
    return this->dataMatrix[x][y];
}

void Datamap::setData(int x, int y, double val) {
    this->dataMatrix[x][y] = val;
}

