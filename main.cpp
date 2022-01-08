#include <iostream>
#include "landscape.hpp"

// Return if completed
void first_trav(Landscape& myLS, int N, bool isRaining) {
    if (isRaining) {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                myLS.receive_new(row, col);  //  1) Receive a new raindrop (if it is still raining) for each point.
                myLS.absorb(row, col);       //  2) If there are raindrops on a point, absorb water into the point
                myLS.cal_trickle(row, col);  //  3a) Calculate the number of raindrops that will next trickle to the lowest neighbor(s)
            }
        }

    }

    else {
        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                myLS.absorb(row, col);       //  2) If there are raindrops on a point, absorb water into the point
                myLS.cal_trickle(row, col);  //  3a) Calculate the number of raindrops that will next trickle to the lowest neighbor(s)
            }
        }
    }
}

void second_trav(Landscape& myLS){
    
    //  3b) For each point, use the calculated number of raindrops that will trickle to the
    //      lowest neighbor(s) to update the number of raindrops at each lowest neighbor, if
    //      applicable.
    myLS.trickle();

}

int main(int argc, char *argv[])
{
    // CLI arguments check
    /** Code here **/

    // Create landscape
    int timestep;

    Landscape myLandscape(A, N);

    // Time passing: raining
    for(timestep=0;timestep<M;++timestep) {
        first_trav(myLandscape, N, true); // Traverse over all landscape points
        second_trav(myLandscape); // Make a second traversal over all landscape points
    
    }

    // Stop raining, continue to absorb and trickle
        first_trav(myLandscape, N, false);
        second_trav(myLandscape);
        timestep++;
    
    
    

    // Print result
    print_result();

    return 0;

}