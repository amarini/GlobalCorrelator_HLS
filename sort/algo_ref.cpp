#include "src/algo.h"
#include <cmath>
#include <algorithm>
#include <vector>


void algo_main_ref(Particle particles[NPARTICLES]) {
    std::vector<Particle> v;
    for(int i=0; i<NPARTICLES;++i){
        v.push_back(particles[i]);
    }
    std::sort(v.begin(),v.end());
    for(int i=0;i<NPARTICLES;++i)
    {
        particles[i] = v[i];
    }
    return ;
}

