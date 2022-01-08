#pragma once

#include <vector>

class Datamap {
    public:
        // Constructor
        Datamap(int n);

        double getData(int x, int y);
        void setData(int x, int y, double val); 

    private:
        int dimension;
        std::vector<std::vector<double>> dataMatrix;
        

        std::vector<std::vector<double>> initDM(int n);

};