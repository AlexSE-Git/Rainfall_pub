#include "utils.hpp"
int STEP = 0;
int M;
double A;
int N;

double simulate(Landscape * myLandscape){
    struct timespec start_time, end_time;
    cout << "Start simulating..."<<endl;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while (true) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (STEP < M) {
                    myLandscape->receive_new(i, j);
                }
                myLandscape->absorb(i, j);
                myLandscape->cal_trickle(i, j);
            }
        }
        STEP++;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                myLandscape->trickle(i, j);
            }
        }
        if (STEP >= M) {
            if (!myLandscape->checkRemain()) {
                break;  // No remain, break out
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_s = calc_time(start_time, end_time) / 1000000000.0;
    return elapsed_s;
}

int main(int argc, char *argv[]){
    M = atoi(argv[2]);
    stringstream ss(argv[3]);
    ss >> A;
    N = atoi(argv[4]);
    string file = argv[5];
    // read file and init myLandscape
    vector<vector<int>> landArray;
    readFile(file, landArray, N); 
    Landscape * myLandscape = new Landscape(A, N, landArray);
    // simulate
    double elapsed_s = simulate(myLandscape);
    // simulate ends, get results
    vector<vector<double>> abs = myLandscape->printAbsorbed();
    cout << "STEP: " << STEP << "  TIME: " << elapsed_s << endl;
    writeFile(STEP, elapsed_s, abs, N);
    delete myLandscape;

    return EXIT_SUCCESS;
}