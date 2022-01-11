#pragma once

#include <vector>

class DataMat {
    public:
        // Constructor
        DataMat(int n);

        double getData(int x, int y) const;
        void setData(int x, int y, double val); 

    private:
        int dimension;
        std::vector<std::vector<double>> dataMatrix;
        

        std::vector<std::vector<double>> initDM(int n);

};