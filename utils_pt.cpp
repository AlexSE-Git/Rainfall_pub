#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "landscape.hpp"
using namespace std;

#define THREAD_NUMBER 8
int RAIN = 0;
int M;
std::vector<std::vector<std::unique_ptr<std::mutex>>> mutexMap;

void initMutex(int N) {
    mutexMap.reserve(N);

    for (int i = 0; i < N; i++) {
        std::vector<std::unique_ptr<std::mutex>> currRow;
        currRow.reserve(N);
        for (int j = 0; j < N; j++) {
            std::unique_ptr<std::mutex> mu_ptr = std::make_unique<std::mutex>();
            currRow.push_back(std::move(mu_ptr));
        }
        mutexMap.push_back(std::move(currRow));
    }
}

double calc_time(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
    double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;
    if (end_sec < start_sec) {
        return 0;
    } else {
        return end_sec - start_sec;
    }
}

void readFile(string& file, vector<vector<int>>& landArray, int N) {
    ifstream ifs;
    ifs.open(file, ifstream::in);

    landArray.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ifs >> landArray[i][j];
        }
    }

    ifs.close();
}

void first_trav(Landscape* ls, int id, int N) {
    int start = id * (N / THREAD_NUMBER);
    int end = (id + 1) * (N / THREAD_NUMBER);

    for (int k = start; k < end; ++k) {
        for (int j = 0; j < N; ++j) {
            if (RAIN < M)
                ls->receive_new(k, j);
            ls->absorb(k, j);
            ls->cal_trickle(k, j);
        }
    }
}

void second_trav(Landscape* ls, int id, int N) {
    int start = id * (N / THREAD_NUMBER);
    int end = (id + 1) * (N / THREAD_NUMBER);

    for (int k = start; k < end; ++k) {
        for (int j = 0; j < N; ++j) {
            ls->p_trickle(k, j, mutexMap);
        }
    }
}

int main(int argc, char** argv) {
    // int P = atoi(argv[1]);
    M = atoi(argv[2]);
    stringstream ss(argv[3]);
    double A;
    ss >> A;
    int N = atoi(argv[4]);

    string file = argv[5];
    vector<vector<int>> landArray;
    struct timespec start_time, end_time;

    readFile(file, landArray, N);

    Landscape* myLandscape = new Landscape(A, N, landArray);

    array<thread, THREAD_NUMBER> pool;

    initMutex(N);

    cout << "Start simulating..." << endl;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while (true) {
        for (int i = 0; i < THREAD_NUMBER; ++i) {
            pool[i] = thread(first_trav, myLandscape, i, N);
        }
        for (int i = 0; i < THREAD_NUMBER; ++i) {
            pool[i].join();
        }

        RAIN++;

        for (int i = 0; i < THREAD_NUMBER; ++i) {
            pool[i] = thread(second_trav, myLandscape, i, N);
        }
        for (int i = 0; i < THREAD_NUMBER; ++i) {
            pool[i].join();
        }

        if (RAIN >= M) {
            if (!myLandscape->checkRemain()) {
                break;  // No remain, break out
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_s = calc_time(start_time, end_time) / 1000000000.0;

    cout << "STEP: " << RAIN << "  TIME: " << elapsed_s << endl;
    vector<vector<double>> abs = myLandscape->printAbsorbed();

    ofstream myfile;
    myfile.open("output_pt.txt");
    myfile << "Rainfall simulation took " << RAIN << " time steps to complete.\n";
    myfile << "Runtime = " << elapsed_s << " seconds\n";
    myfile << "\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            myfile << abs[i][j] << " ";
        }
        myfile << "\n";
    }
    myfile.close();

    delete myLandscape; // Release heap resource

    return EXIT_SUCCESS;
}