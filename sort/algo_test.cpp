#include <cstdio>
#include "src/data.h"
#include "src/algo.h"

#define NTEST 500

void printArray(Particle arr[])
{
    for(int i=0;i<NPARTICLES;++i)
    {
        //printf("(%u,%u) ",arr[i].hwPt,arr[i].hwEta);
        printf("%u ",unsigned(arr[i].hwPt));
    }
    printf("\n");
}

int main() {
    srand(42);
    Particle particles[NPARTICLES];
    Particle particles2[NPARTICLES];

    for (int test = 1; test <= NTEST; ++test) {
        for (int i = 0; i < NPARTICLES; ++i) {
            //particles[i].hwPt  = int((rand()/float(RAND_MAX) * 20 + 4)/0.25);
            particles[i].hwPt  = rand() %100;  //DEBUG
            particles[i].hwEta = int((rand()/float(RAND_MAX/2) - 1)*5 / 0.01);
	    
            particles2[i].hwPt = particles[i].hwPt; // make a copy
            particles2[i].hwEta = particles[i].hwEta;

        }
        printf("inputs:\n");
        printArray(particles);
        printArray(particles2);

        for (int i = 0; i < NPARTICLES; ++i) {
            if (    (particles[NPARTICLES-1-i].hwPt != particles2[NPARTICLES-1-i].hwPt)
                 or (particles[NPARTICLES-1-i].hwEta != particles2[NPARTICLES-1-i].hwEta)
                    ){
                printf("WTF inputs are different %d\n", test);
                return 1;
            }
        }

        // run the algorithm
        algo_main(particles,MPARTICLES);
        algo_main_ref(particles2);
        
        printf("main:\n");
        printArray(particles);
        printf("ref:\n");
        printArray(particles2);

        // check the output
        for (int i =1; i<= MPARTICLES ; ++i)
        {
            if (    (particles[NPARTICLES-i].hwPt != particles2[NPARTICLES-i].hwPt)
                 or (particles[NPARTICLES-i].hwEta != particles2[NPARTICLES-i].hwEta)
                    ){
                printf("Error in test %d\n", test);
                printf("Particle %d of %d greatest (%d total) \n", i, MPARTICLES,NPARTICLES);
                printf("* pt = %u eta=%u\n",unsigned(particles[NPARTICLES-i].hwPt),unsigned(particles[NPARTICLES-i].hwEta));
                printf("* (ref) pt = %u eta=%u\n",unsigned(particles2[NPARTICLES-i].hwPt),unsigned(particles2[NPARTICLES-i].hwEta));
                return 1;
            }
        }
    }
    printf("Passed all %d tests\n", NTEST);
    return 0;
}
