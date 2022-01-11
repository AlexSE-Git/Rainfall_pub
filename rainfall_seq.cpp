#include "utils.hpp"
int STEP = 0;

// M = # of simulation time steps during which a rain drop will fall on each landscape
// point. In other words, 1 rain drop falls on each point during the first M steps of the
// simulation.
int M;

// A = absorption rate (specified as a floating point number). The amount of raindrops
// that are absorbed into the ground at a point during a timestep.
double A;

// N = dimension of the landscape (NxN)
int N;

double simulate(Landscape& myLandscape){
    struct timespec start_time, end_time;
    cout << "Start simulating..."<<endl;
    // Simulation starts, start clock
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    while (true) {

        // First traversal
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (STEP < M) {
                    myLandscape.receive_new(i, j);
                }
                myLandscape.absorb(i, j);
                myLandscape.cal_trickle(i, j);
            }
        }

        STEP++;

        // Second traversal
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                myLandscape.trickle(i, j);
            }
        }
        if (STEP >= M) {
            if (!myLandscape.checkRemain()) {
                break;  // No remain, break out
            }
        }

    }

    // Simulation ends, stop clock
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_s = calc_time(start_time, end_time) / 1000000000.0;
    return elapsed_s;
}

int main(int argc, char *argv[]){
    M = atoi(argv[2]);

    stringstream ss(argv[3]);
    ss >> A;

    N = atoi(argv[4]);

    // elevation_file = name of input file that specifies the elevation of each point.
    string elevation_file = argv[5];

    // read file and init myLandscape
    vector<vector<int>> landArray;
    readFile(elevation_file, landArray, N); 
    Landscape myLandscape = Landscape(A, N, landArray);

    // simulate
    double elapsed_s = simulate(myLandscape);

    // simulate ends, get results
    vector<vector<double>> abs = myLandscape.printAbsorbed();
    cout << "STEP: " << STEP << "  TIME: " << elapsed_s << endl;
    writeFile(STEP, elapsed_s, abs, N);

    return EXIT_SUCCESS;
}