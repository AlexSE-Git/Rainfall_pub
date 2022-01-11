#include "DataMat.hpp"
#include <iostream>

DataMat::DataMat(int n) {
    this->dimension = n;
    this->dataMatrix = this->initDM(n);
}

std::vector<std::vector<double>> DataMat::initDM(int n) {
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

double DataMat::getData(int x, int y) const{
    return this->dataMatrix[x][y];
}

void DataMat::setData(int x, int y, double val) {
    this->dataMatrix[x][y] = val;
}

