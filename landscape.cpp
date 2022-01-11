#include "landscape.hpp"

#include <iostream>

#include "DataMat.hpp"
#include "VectorMat.hpp"

Landscape::Landscape(double abr, int n, const vector<vector<int>>& landArray) : rainMat(n), nextTrickleMat(n), absorbedMat(n), absorption_rate(abr), dimension(n), directions(n) {
    // Store elevation info in our data structure: vectormat
    this->directions.setData(landArray, n);
}

void Landscape::receive_new(int x, int y) {
    // Add 1 new rain drop on desinated coordinate
    double currD = this->rainMat.getData(x, y);
    this->rainMat.setData(x, y, currD + 1);
}

void Landscape::absorb(int x, int y) {
    double currR = this->rainMat.getData(x, y);
    double currA = this->absorbedMat.getData(x, y);
    double realAbsorbed = std::min(this->absorption_rate, currR);

    this->rainMat.setData(x, y, currR - realAbsorbed);
    this->absorbedMat.setData(x, y, currA + realAbsorbed);
}

void Landscape::cal_trickle(int x, int y) {
    double curr = this->rainMat.getData(x, y);

    // Trickle 1 drop at most
    this->nextTrickleMat.setData(x, y, std::min(1.0, curr));
}

std::vector<std::vector<double>> Landscape::printAbsorbed() const{
    std::vector<std::vector<double>> out;

    for (int row = 0; row < this->dimension; row++) {
        std::vector<double> currRow;
        for (int col = 0; col < this->dimension; col++) {
            currRow.push_back(this->absorbedMat.getData(row, col));
        }
        out.push_back(currRow);
    }

    return out;
}

void Landscape::trickle(int row, int col) {
    double trickle_total = nextTrickleMat.getData(row, col);

    if (!directions.ifTrickle(row, col) || trickle_total == 0) {
        return;
    }

    vector<int> x_array = directions.getX(row, col);
    vector<int> y_array = directions.getY(row, col);
    double trickle_num = x_array.size(); // Number of places to be trickle evenly
    double trickle_amount = trickle_total / trickle_num; // Each place will receive same fraction of rain

    // Update all receiving ends
    for (int i = 0; i < trickle_num; i++) {
        int r = row + x_array[i];
        int c = col + y_array[i];
        double currD = rainMat.getData(r, c);
        rainMat.setData(r, c, currD + trickle_amount);
    }

    // Update the giving end
    double curr_rain = rainMat.getData(row, col);
    rainMat.setData(row, col, curr_rain - trickle_total);
}

void Landscape::p_trickle(int row, int col, std::vector<std::vector<std::unique_ptr<std::mutex>>>& mMap) {
    double trickle_total = nextTrickleMat.getData(row, col);

    if (!directions.ifTrickle(row, col) || trickle_total == 0) {
        return;
    }

    vector<int> x_array = directions.getX(row, col);
    vector<int> y_array = directions.getY(row, col);
    double trickle_num = x_array.size();
    double trickle_amount = trickle_total / trickle_num;
    for (int i = 0; i < trickle_num; i++) {
        int r = row + x_array[i];
        int c = col + y_array[i];

        // Critical section for receiving ends
        mMap[r][c]->lock();
        double currD = rainMat.getData(r, c);
        rainMat.setData(r, c, currD + trickle_amount);
        mMap[r][c]->unlock();

    }

    // Critical section for giving end
    mMap[row][col]->lock();
    double curr_rain = rainMat.getData(row, col);
    rainMat.setData(row, col, curr_rain - trickle_total);
    mMap[row][col]->unlock();
}

void Landscape::printRain() const{
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < this->dimension; j++) {
            std::cout << rainMat.getData(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

bool Landscape::checkRemain() const{
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < this->dimension; j++) {
            if (this->rainMat.getData(i, j) != 0.0) {
                return true;
            }
        }
    }
    return false;
}

double Landscape::getRemain(int x, int y) const{
    return this->rainMat.getData(x, y);
}