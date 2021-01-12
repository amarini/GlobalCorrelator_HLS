//    Every node has a colour either red or black.
//    The root of tree is always black.
//    There are no two adjacent red nodes (A red node cannot have a red parent or red child).
//   Every path from a node (including root) to any of its descendant NULL node has the same number of black nodes.

// define colors
#ifndef RB_TREE_H
#define RB_TREE_H

// height = sum colors :D 
#define BLACK 1
#define RED 0

template <typename T>
inline T max(T a, T b)  
{  
    return (a < b)? b : a;  
}  

template <typename T>
inline T min(T a, T b)  
{  
    return (a < b)? a : b;  
}  

template<typename T>
void myswap(T& a, T&b)
{
	T c = a;
	a=b;
	b=c;
	return;
}

template <typename T>
void compAndSwap(T a[], int i, int j, int dir) 
{ 
    if (dir==(not (a[i]<a[j])))  // check ==
        myswap(a[i],a[j]); 
} 

/*It recursively sorts a bitonic sequence in ascending order, 
  if dir = 1, and in descending order otherwise (means dir=0). 
  The sequence to be sorted starts at index position low, 
  the parameter cnt is the number of elements to be sorted.*/
template<typename T>
void bitonicMerge(T a[], int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
        for (int i=low; i<low+k; i++)  compAndSwap(a, i, i+k, dir); 
        bitonicMerge(a, low, k, dir); 
        bitonicMerge(a, low+k, k, dir); 
    } 
} 

/* This function first produces a bitonic sequence by recursively 
    sorting its two halves in opposite sorting orders, and then 
    calls bitonicMerge to make them in the same order */
template <typename T>
void bitonicSort(T a[],int low, int cnt, int dir) 
{ 
    if (cnt>1) 
    { 
        int k = cnt/2; 
  
        // sort in ascending order since dir here is 1 
        bitonicSort(a, low, k, 1); 
  
        // sort in descending order since dir here is 0 
        bitonicSort(a, low+k, k, 0); 
  
        // Will merge wole sequence in ascending order 
        // since dir=1. 
        bitonicMerge(a,low, cnt, dir); 
    } 
} 

/* Caller of bitonicSort for sorting the entire array of 
   length N in ASCENDING order */
template<typename T>
void sort(T a[], int n) 
{ 
   // TODO check: likely needs to be n==2^i
    bitonicSort(a,0, n, 1); 
} 

int get_pow2_size(unsigned int n){
    if (n && !(n&(n-1)))return n;
    int count=0;
    while(n) {n>>=1;++count;}
    return 1<<count;
}

template<typename T>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[], T out[], T zero=0) 
{ 
    std::cout <<"MY SORT ================"<<std::endl;
   // TODO check: likely needs to be n==2^i if (nIn && !(nIn & (nIn - 1))) //ok
   int n=get_pow2_size(nIn);
   T a[n];
   for (int i=0;i<n;++i) {if (i<nIn) a[i] = in[i]; else a[i] = zero;}
   bitonicSort(a,0, n, 1); 
   // copy in out
   for (int i=0;i<nOut;++i){out[i] = a[n-i];}
   return;
} 

//void puppisort_and_crop_ref(unsigned int nIn, unsigned int nOut, const PuppiObj in[/*nIn*/], PuppiObj out[/*nOut*/]) {

#endif

//from https://en.wikipedia.org/wiki/Bitonic_sorter
//    // given an array arr of length n, this code sorts it in place
//    // all indices run from 0 to n-1
//    for (k = 2; k <= n; k *= 2) // k is doubled every iteration
//        for (j = k/2; j > 0; j /= 2) // j is halved at every iteration, with truncation of fractional parts
//            for (i = 0; i < n; i++)
//                l = bitwiseXOR (i, j); // in C-like languages this is "i ^ j"
//                if (l > i)
//                    if (  (bitwiseAND (i, k) == 0) AND (arr[i] > arr[l])
//                       OR (bitwiseAND (i, k) != 0) AND (arr[i] < arr[l]) )
//                          swap the elements arr[i] and arr[l]
