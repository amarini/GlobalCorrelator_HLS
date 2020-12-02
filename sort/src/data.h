#ifndef ALGO_DATA_H
#define ALGO_DATA_H

#include "ap_int.h"

typedef ap_int<16>  pt_t;      // 1 unit = 0.25 GeV; max = 8 TeV
typedef ap_int<10>  etaphi_t;  // 1 unit = 0.01;     max = 5.12


struct Particle {
    pt_t hwPt;
    etaphi_t hwEta; 
};

#define NPARTICLES 20
#define MPARTICLES 5

#endif
