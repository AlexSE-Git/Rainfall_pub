#include "landscape.hpp"

#include <iostream>

#include "datamap.hpp"
#include "vectormap.hpp"

Landscape::Landscape(double abr, int n, const vector<vector<int>>& landArray) : rainMap(n), nextTrickleMap(n), absorbedMap(n), absorption_rate(abr), dimension(n), directions(n) {
    this->directions.setData(landArray, n);
}

void Landscape::receive_new(int x, int y) {
    double currD = this->rainMap.getData(x, y);
    this->rainMap.setData(x, y, currD + 1);
}

void Landscape::absorb(int x, int y) {
    double currR = this->rainMap.getData(x, y);
    double currA = this->absorbedMap.getData(x, y);
    double realAbsorbed;

    if (currR < this->absorption_rate) {
        realAbsorbed = currR;
    } else {
        realAbsorbed = this->absorption_rate;
    }

    this->rainMap.setData(x, y, currR - realAbsorbed);
    this->absorbedMap.setData(x, y, currA + realAbsorbed);
}

void Landscape::cal_trickle(int x, int y) {
    double curr = this->rainMap.getData(x, y);

    // Trickle 1 drop at most
    if (curr > 1.0) {
        this->nextTrickleMap.setData(x, y, 1.0);
    } else {
        this->nextTrickleMap.setData(x, y, curr);
    }
}

std::vector<std::vector<double>> Landscape::printAbsorbed() {
    std::vector<std::vector<double>> out;

    for (int row = 0; row < this->dimension; row++) {
        std::vector<double> currRow;
        for (int col = 0; col < this->dimension; col++) {
            currRow.push_back(this->absorbedMap.getData(row, col));
        }
        out.push_back(currRow);
    }

    return out;
}

void Landscape::trickle(int row, int col) {
    double trickle_total = min(nextTrickleMap.getData(row, col), 1.0);
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
        double currD = rainMap.getData(r, c);
        rainMap.setData(r, c, currD + trickle_amount);
    }
    double curr_rain = rainMap.getData(row, col);
    rainMap.setData(row, col, curr_rain - trickle_total);
}

void Landscape::p_trickle(int row, int col, std::vector<std::vector<std::unique_ptr<std::mutex>>>& mMap) {
    double trickle_total = min(nextTrickleMap.getData(row, col), 1.0);
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
        mMap[r][c]->lock();
        double currD = rainMap.getData(r, c);
        rainMap.setData(r, c, currD + trickle_amount);
        mMap[r][c]->unlock();
    }
    mMap[row][col]->lock();
    double curr_rain = rainMap.getData(row, col);
    rainMap.setData(row, col, curr_rain - trickle_total);
    mMap[row][col]->unlock();
}

void Landscape::printRain() {
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < this->dimension; j++) {
            cout << rainMap.getData(i, j) << " ";
        }
        cout << endl;
    }
}

bool Landscape::checkRemain() {
    for (int i = 0; i < this->dimension; i++) {
        for (int j = 0; j < this->dimension; j++) {
            if (this->rainMap.getData(i, j) != 0.0) {
                return true;
            }
        }
    }
    return false;
}