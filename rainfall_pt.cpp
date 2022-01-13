#include "utils.hpp"
int STEP = 0;
int M;
double A;
int N;
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

void first_trav(Landscape& ls, int id, int P) {
    int start = id * (N / P);
    int end = (id + 1) * (N / P);

    for (int k = start; k < end; ++k) {
        for (int j = 0; j < N; ++j) {
            if (STEP < M)
                ls.receive_new(k, j);
            ls.absorb(k, j);
            ls.cal_trickle(k, j);
        }
    }
}

void second_trav(Landscape& ls, int id, int P) {
    int start = id * (N / P);
    int end = (id + 1) * (N / P);

    for (int k = start; k < end; ++k) {
        for (int j = 0; j < N; ++j) {
            ls.p_trickle(k, j, mutexMap);
        }
    }
}

float simulate(Landscape& myLandscape, char *argv[]){
    using namespace std::chrono;

    const int P = atoi(argv[1]);
    initMutex(N);
    std::thread pool[P];
    std::cout << "Start simulating..."<<std::endl;
    // Simulation starts, start clock
    auto start = high_resolution_clock::now();
    while (true) {
        if (N<P){ // handle the situation where N is smaller than P
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
            continue;
        }
        for (int i = 0; i < P; ++i) {
            pool[i] = std::thread(first_trav, std::ref(myLandscape), i, P);
        }
        for (int i = 0; i < P; ++i) {
            pool[i].join();
        }
        STEP++;
        for (int i = 0; i < P; ++i) {
            pool[i] = std::thread(second_trav, std::ref(myLandscape), i, P);
        }
        for (int i = 0; i < P; ++i) {
            pool[i].join();
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

    using std::vector;
    
    M = atoi(argv[2]);
    std::stringstream ss(argv[3]);
    ss >> A;
    N = atoi(argv[4]);
    std::string file = argv[5];
    // read file and init myLandscape
    vector<vector<int>> landArray;
    readFile(file, landArray, N); 
    Landscape myLandscape(A, N, landArray);
    // simulate
    float elapsed_s = simulate(myLandscape, argv);
    // simulate ends, get results
    vector<vector<double>> abs = myLandscape.printAbsorbed();
    std::cout << "STEP: " << STEP << "  TIME: " << elapsed_s << std::endl;
    writeFile(STEP, elapsed_s, abs, N);

    return EXIT_SUCCESS;
}