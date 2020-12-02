#include "algo.h"


void algo_main(Particle particles[NPARTICLES],int m) {
    #pragma HLS ARRAY_PARTITION variable=particles complete
    #pragma HLS pipeline II=1
    // need operator< 
    partial_heap_sort(particles,NPARTICLES,m);
    return ;
}


