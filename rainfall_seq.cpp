#include "utils.hpp"
#include <chrono>
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

float simulate(Landscape& myLandscape){
    using namespace std::chrono;

    std::cout << "Start simulating..."<<std::endl;
    // Simulation starts, start clock
    auto start = high_resolution_clock::now();

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
    auto end = high_resolution_clock::now();

    duration<float> duration = end - start;
    return duration.count();
}

int main(int argc, char *argv[]){
    M = atoi(argv[2]);

    std::stringstream ss(argv[3]);
    ss >> A;

    N = atoi(argv[4]);

    // elevation_file = name of input file that specifies the elevation of each point.
    std::string elevation_file = argv[5];

    // read file and init myLandscape
    vector<vector<int>> landArray;
    readFile(elevation_file, landArray, N); 
    Landscape myLandscape(A, N, landArray);

    // simulate
    float elapsed_s = simulate(myLandscape);

    // simulate ends, get results
    vector<vector<double>> abs = myLandscape.printAbsorbed();
    std::cout << "STEP: " << STEP << "  TIME: " << elapsed_s << std::endl;
    writeFile(STEP, elapsed_s, abs, N);

    return EXIT_SUCCESS;
}

 
//     struct Timer
// {
//     time_point<steady_clock> start, end;
//     duration<float> duration;
//     Timer()
//     {
//         start = high_resolution_clock::now();
//     }

//     ~Timer(){
//         end = high_resolution_clock::now();
//         duration = end - start;

//         float ms = duration.count() * 1000.0f;
//         std::cout << "Timer took" << ms << "ms " << std::endl;
//     }
// };