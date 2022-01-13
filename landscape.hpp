#pragma once

#include "DataMat.hpp"
#include "VectorMat.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <algorithm>


class Landscape {
    public:
        // Constructor
        // Parameters: absorb rate, dimension, land array
        Landscape(double abr, int n, const std::vector<std::vector<int>> & landArray); 

        void receive_new(int x, int y);
        void absorb(int x, int y);
        void cal_trickle(int x, int y); // Calculate and update nextTrickleMat ready for trickle
        void trickle(int row, int col);
        void p_trickle(int row, int col, std::vector<std::vector<std::unique_ptr<std::mutex>>>& mMap); // Parallel version of trickle

        std::vector<std::vector<double>> printAbsorbed() const;
        void printRain() const; 
        bool checkRemain() const;
        double getRemain(int x, int y) const;

    private:
        DataMat rainMat;
        DataMat nextTrickleMat;
        DataMat absorbedMat;

        double absorption_rate;
        int dimension;

        VectorMat directions; // Our custom data structure to store elevation info
};