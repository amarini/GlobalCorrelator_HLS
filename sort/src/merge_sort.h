// Function to merge the two haves arr[l..m]
// and arr[m+1..r] of array arr[] 
template<typename T>
void merge(T in1[], T in2[], T out[], int InSize1, int InSize2, int OutSize)
{
    int l=0,r=0; // position in the 1/2 array
    for( int i=0;i<OutSize;++i){
        #pragma HLS pipeline II=1
        if (l>=InSize1 && r >=InSize2) {
            //std::cout<<"[ERROR]"<<__FUNCTION__<<std::endl;
            continue; // throw error
        }
        if (l>=InSize1) {out[i] = in2[r++]; continue;}
        if (r>=InSize2) {out[i] = in1[l++]; continue;}
        if (in1[l] < in2[r]) { 
            out[i] = in2[r++]; 
        } else {  // >=
            out[i] = in1[l++];
        }
    }
}
 
// l is for left index and r is
// right index of the sub-array
// of arr to be sorted 
template <typename T>
void mergeSort(T in[], T out[], int InSize, int OutSize) // InSize = OutSize
{
    for(int size=1;size < /*2*/InSize ; size*=2 ) // miss last
    {
    //#pragma HLS pipeline II=1
        // 2 chunks of size are being merged
        for(int l=0;l<InSize;l+=2*size){
            #pragma HLS unroll
            int leftsize=(l+size<=InSize)?size:(InSize -l);
            int rightsize=(l+leftsize+size<=InSize)?size:(InSize-l-leftsize);
            merge(in+l, in+l+leftsize, out+l, leftsize, rightsize, leftsize+rightsize); 
        }

        // copy back to in for next iteration
        if(size <= InSize){
            for(int i=0;i<InSize;++i){
                #pragma HLS unroll
                in[i] = out[i];
            }
        }
    }

}

    template<typename T,int NP>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[], T out[]) 
{  // just an interface
    T outTmp[NP];
    mergeSort(in,outTmp,nIn,nIn); 
    for(int i=0;i<nOut;++i){
        #pragma HLS unroll
        out[i] = outTmp[i];
    }
}
 
