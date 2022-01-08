#pragma once

#include "datamap.hpp"
#include "vectormap.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

class Landscape {
    public:
        // Constructor
        Landscape(double abr, int n, const vector<vector<int>> & landArray); 

        void receive_new(int x, int y);
        void absorb(int x, int y);
        void cal_trickle(int x, int y); // Update nextTrickleMap ready for trickle
        void trickle(int row, int col);
        void p_trickle(int row, int col, std::vector<std::vector<std::unique_ptr<std::mutex>>>& mMap);

        std::vector<std::vector<double>> printAbsorbed();
        void printRain(); 
        bool checkRemain();
        double checkRemain(int x, int y);

    private:
        Datamap rainMap;
        Datamap nextTrickleMap;
        Datamap absorbedMap;

        double absorption_rate;
        int dimension;

        VectorMap directions; /** need definition **/
};