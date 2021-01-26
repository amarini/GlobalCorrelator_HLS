#ifndef MERGE_SORT_H
#define MERGE_SORT_H

template<typename T, int NP>
void merge(T in[NP], T out[NP], int InSize1, int InSize2, int OutSize, int st1, int st2, int sto)
{
     #pragma HLS inline
    int l=0,r=0; // position in the 1/2 array
    for( int i=0;i<OutSize;++i){
        #pragma HLS pipeline II=1
        if (l>=InSize1 && r >=InSize2) {
            //std::cout<<"[ERROR]"<<__FUNCTION__<<std::endl;
            continue; // throw error
        }
        if (l>=InSize1) {out[sto+i] = in[st2+r++]; continue;}
        if (r>=InSize2) {out[sto+i] = in[st1+l++]; continue;}
        if (in[st1+l] < in[st2+r]) { 
            out[sto+i] = in[st2+r++]; 
        } else {  // >=
            out[sto+i] = in[st1+l++];
        }
    }
}

template<typename T, int Size>
void copy(T in[], T out[],int start=0)
{
     #pragma HLS inline
    for(int i=0;i<Size;++i)
    {
     #pragma HLS unroll
        out[start+i] = in[start+i];
    }
}

template <typename T,int InSize,int size>
void mergeSort(T in[InSize], T out[InSize], int start) 
{
    //std::cout<<"[DEBUG]"<<__FUNCTION__<<" InSize="<<InSize<<" size="<<size<<" start="<<start<<" @in in="; printArrayDebug(in,size,start);
    //copy<T,InSize>(in,out); // can be removed if controlled copy from out->in

     #pragma HLS inline
    static constexpr int halfsize=size/2;
    static constexpr int halfsize2=size-size/2;
    mergeSort<T,InSize,halfsize>(in,out,start);
    mergeSort<T,InSize,halfsize2>(in,out,start+halfsize);

    copy<T,size>(out,in,start);

    //std::cout<<"[DEBUG]"<<__FUNCTION__<<" InSize="<<InSize<<" size="<<size<<" start="<<start<<" @mid in="; printArrayDebug(in,size, start);
    //copy<T,size>(out,in,start); // FAIL
    merge<T,InSize>(in,out,halfsize,halfsize2,size, start, start+halfsize, start);

    //std::cout<<"[DEBUG]"<<__FUNCTION__<<" InSize="<<InSize<<" size="<<size<<" start="<<start<<" @end out="; printArrayDebug(out,size, start);
}

//template <typename T,int InSize>
//void mergeSort<T,InSize,1>(T in[InSize], T out[InSize], int start ) 
//{
//    out[start]=in[start];
//    return;
//}

// N is defined  trough a define
template<>
void mergeSort<int,N,1>(int in[N], int out[N], int start ) 
{
     #pragma HLS inline
    out[start]=in[start];
    return;
}


    template<typename T,int NP>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[NP], T out[NP]) 
{  // just an interface
    T outTmp[NP];
    mergeSort<T,NP,NP>(in,outTmp,0); 
    for(int i=0;i<NP;++i){
        #pragma HLS unroll
        if (i<nOut)out[i] = outTmp[i];
    }
}
#endif 
