#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include "landscape.hpp"

using std::vector;

void readFile(std::string& file, vector<vector<int>>& landArray, int N) {
    std::ifstream ifs;
    ifs.open(file, std::ifstream::in);

    landArray.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ifs >> landArray[i][j];
        }
    }

    ifs.close();
}

void writeFile(int STEP, double elapsed_s, vector<vector<double>> &abs, int N){
    std::ofstream myfile;
    myfile.open ("output.txt");
    myfile << "Rainfall simulation took "<< STEP <<" time steps to complete.\n";
    myfile << "Runtime = "<< elapsed_s <<" seconds\n";
    myfile << "\n";
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            myfile << abs[i][j] << " ";
        }
        myfile << "\n";
    }
    myfile.close(); 
}

// double calc_time(struct timespec start, struct timespec end) {
// 	double start_sec = (double)start.tv_sec*1000000000.0 + (double)start.tv_nsec;
// 	double end_sec = (double)end.tv_sec*1000000000.0 + (double)end.tv_nsec;
// 	if (end_sec < start_sec) {
// 		return 0;
// 	} else {
// 		return end_sec - start_sec;
// 	}
// }