#ifndef ALGO_H
#define ALGO_H

#include "data.h"
//#include "sort.h"
#include "avl_tree.h"

inline const bool operator<(const Particle &a, const Particle&b){if (a.hwPt ==b.hwPt) return a.hwEta<b.hwEta; return a.hwPt <b.hwPt;}
//const bool operator>(const Particle &a, const Particle&b){return b<a;}

// implementation to be synthethised
void algo_main(Particle particles[NPARTICLES],int m) ;

// reference implementation for validation
void algo_main_ref(Particle particles[NPARTICLES]) ;

#endif
