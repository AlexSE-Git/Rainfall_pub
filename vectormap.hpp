#pragma once
#include <array>
#include <vector>
#include <map>
using namespace std;

class VectorMap {
    public:
        
        // Constructor
        VectorMap(int N);

        // get the vector of trickle (x, y)s
        vector<int> getX(int x, int y);
        vector<int> getY(int x, int y);

        // set the vector map using land height matrix
        void setData(const vector<vector<int>> & landArray, int & N);

        // check if will trickle
        bool ifTrickle(int x, int y);

        ~VectorMap(){}

    private: 
        vector<vector<int>> directions{{0,1}, {0,-1}, {1,0}, {-1,0}};
        
        vector<vector<vector<int>>> X;
        vector<vector<vector<int>>> Y;
        
        vector<vector<bool>> trickleMatrix;
};