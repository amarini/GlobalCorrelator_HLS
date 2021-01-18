#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>      // std::setw
#include <algorithm>
//#include "src/avl_tree.h"
#include "src/bitonic_sort_new.h"

template<typename T>
void printArray(T arr[],int n) // DEBUG
{
    //int n = sizeof(arr)/sizeof(arr[0]);
    std::cout<<n<<" | "; 
    for(int i=0;i<n;++i)
    {
        std::cout<<std::setw(4)<<arr[i]<<" ";
    }
    std::cout<<std::endl;
}

//#define NTEST 100
#define NTEST 100

#define N 50
//#define N 32
#define M 32

//#define N 7
//#define M 4

int main() {
    srand(42);
    bool pass=true;
    for (int test = 1; test <= NTEST; ++test) {
        int arr[N];
        int arr2[N];
        for(int i=0;i<N;++i ) {arr[i] = rand()%100;}
        arr[0] = 1; arr[1]=2; arr[2] = 3 ; arr[3] = 4; // RR rotation

        for(int i=0;i<N;++i ) {arr2[i] = arr[i];} // copy for std::sort
        std::cout<<"INPUT | ";printArray(arr,N);

        //partial_avl_sort(arr,N,-1);
        int out[M];
        sort_and_crop_ref(N, M, arr, out) ;
        std::cout<<"OUTPUT | ";printArray(out,M);

	    std::sort(arr2,arr2+N,std::greater<int>());
        std::cout<<"STDSORT | ";printArray(arr2,N);
        
        for(int i=0;i<M;++i) if (out[i] != arr2[i])pass=false;

    }
    if (pass) std::cout<<" --------------- OK ----------------" <<std::endl;
    else      std::cout<<" ------------   FAIL   -------------" <<std::endl;
    return 0;
}
