#include "VectorMat.hpp"

VectorMat::VectorMat(int N) {
    X.assign(N, vector<vector<int>>(N, vector<int>(0)));
    Y.assign(N, vector<vector<int>>(N, vector<int>(0)));
    trickleMatrix.assign(N, vector<bool>(N, false));
}

vector<int> VectorMat::getX(int x, int y){
    return this->X[x][y];
}

vector<int> VectorMat::getY(int x, int y){
    return this->Y[x][y];
}

bool VectorMat::ifTrickle(int x, int y){
    return this->trickleMatrix[x][y];
}

void VectorMat::setData(const vector<vector<int>> & landArray, int & N){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            int minHeight = landArray[i][j]-1;
            for(int d=0; d<4; d++){
                int r = i+directions[d][0];
                int c = j+directions[d][1];
                if(r>=0 && r<N && c>=0 && c<N){
                    if (landArray[r][c]<minHeight){
                        minHeight = landArray[r][c];
                        if(!X[i][j].empty()){
                            X[i][j].clear();
                            Y[i][j].clear();
                        }
                        X[i][j].push_back(directions[d][0]);
                        Y[i][j].push_back(directions[d][1]);
                    } 
                    else if(landArray[r][c]==minHeight){
                        X[i][j].push_back(directions[d][0]);
                        Y[i][j].push_back(directions[d][1]);
                    }
                }
            }
            if(X[i][j].size()!=0){
                trickleMatrix[i][j] = true;
            }
        }
    }
}