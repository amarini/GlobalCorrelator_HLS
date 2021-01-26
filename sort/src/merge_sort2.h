//INFO: [XFORM 203-502] Unrolling all sub-loops inside loop 'Loop-1' (./../sort/merge_sort.h:13) in function 'mergeSort<PuppiObj, 50>' for pipelining.
//INFO: [HLS 200-489] Unrolling loop 'Loop-1' (./../sort/merge_sort.h:60) in function 'sort_and_crop_ref<PuppiObj, 50>' completely with a factor of 50.
//WARNING: [XFORM 203-503] Cannot unroll loop 'Loop-1.1' (./../sort/merge_sort.h:17) in function 'mergeSort<PuppiObj, 50>' completely: variable loop bound.
//WARNING: [XFORM 203-503] Cannot unroll loop 'Loop-1.1.1' (./../sort/merge_sort.h:26) in function 'mergeSort<PuppiObj, 50>': cannot completely unroll a loop with a variable trip count.
 
// l is for left index and r is
// right index of the sub-array
// of arr to be sorted 
//

//echo -n "{"; for i in {1..100}; do echo "l($i)/l(2)" | bc -l | xargs printf "%.0f" | tr -d '\n' ; echo -n "," ; done; echo "}"
//echo -n "{"; for i in {0..10}; do echo "2^$i" | bc | tr -d '\n' ; echo -n "," ; done; echo "}"

////floor(log2)
//const int log2[]={-1,0,1,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7};
//// 2^i
//const int exp2[]={1,2,4,8,16,32,64,128,256,512,1024};

template <typename T,int InSize>
void mergeSort(T in[InSize], T out[InSize]) //, int InSize, int OutSize) // InSize = OutSize
{
    //for (int is=0; is<log2[InSize]; ++is) // write in this way to allow latency calculation?
    //int size=exp2[is];
    for(int size=1;size < InSize ; size*=2 ) 
    {
    #pragma HLS pipeline II=1
        // 2 chunks of size are being merged
        for(int l=0;l<InSize;l+=2*size){
            #pragma HLS unroll
            int leftsize=(l+size<=InSize)?size:(InSize -l);
            int rightsize=(l+leftsize+size<=InSize)?size:(InSize-l-leftsize);
                    //merge<T,InSize,2*size>(in, out,leftsize,rightsize,leftsize+rightsize, l,l+leftsize,l); 
                    //template<typename T, int NP, int OutSize2>
                    //void merge(T in[NP], T out[NP], int InSize1, int InSize2, int OutSize, int st1, int st2, int sto)
                    {
                        int il=0,ir=0; // position in the 1/2 array
                        for( int i=0;i<InSize;++i){
                            #pragma HLS pipeline II=1
                            if (i>=leftsize+rightsize) continue;  // latency?
                            if (il>=leftsize && ir >=rightsize) {
                                //std::cout<<"[ERROR]"<<__FUNCTION__<<std::endl;
                                continue; // throw error
                            }
                            if (il>=leftsize) {out[l+i] = in[l+leftsize+ir++]; continue;}
                            if (ir>=rightsize) {out[l+i] = in[l+il++]; continue;}
                            if (in[l+il] < in[l+leftsize+ir]) { 
                                out[l+i] = in[l+leftsize+ir++]; 
                            } else {  // >=
                                out[l+i] = in[l+il++];
                            }
                        }
                    }
        }

        // copy back to in for next iteration (remove condition for latency accounting ?)
        if(size <= InSize){
            for(int i=0;i<InSize;++i){
                #pragma HLS unroll
                in[i] = out[i];
            }
        }
    }

}

    template<typename T,int NPARTICLES>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[NPARTICLES], T out[NPARTICLES]) 
{  // just an interface
    T outTmp[NPARTICLES];
    mergeSort<T,NPARTICLES>(in,outTmp); 
    for(int i=0;i<NPARTICLES;++i){
        #pragma HLS unroll
        if (i<nOut)out[i] = outTmp[i];
    }
}
 
