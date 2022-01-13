#pragma once
#include <array>
#include <vector>
#include <map>

using std::vector;

class VectorMat {
    public:
        
        // Constructor
        VectorMat(int N);

        // get the vector of trickle (x, y)s
        std::vector<int> getX(int x, int y);
        std::vector<int> getY(int x, int y);

        // set the vector map using land height matrix
        void setData(const std::vector<std::vector<int>> & landArray, int & N);

        // check if will trickle
        bool ifTrickle(int x, int y);

        ~VectorMat(){}

    private: 
        std::vector<std::vector<int>> directions{{0,1}, {0,-1}, {1,0}, {-1,0}};
        
        std::vector<std::vector<std::vector<int>>> X;
        std::vector<std::vector<std::vector<int>>> Y;
        
        std::vector<std::vector<bool>> trickleMatrix;
};