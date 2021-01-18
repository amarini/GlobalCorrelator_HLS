// define colors
#ifndef BITONIC_NEW_H
#define BITONIC_NEW_H

// master switch
#define DEBUG 0


#define printDebug if(DEBUG>0)std::cout<<"[DEBUG]:"<<__FUNCTION__<<" Begin"<<std::endl;
#define printEndDebug if(DEBUG>0)std::cout<<"[DEBUG]:"<<__FUNCTION__<<" End"<<std::endl;

#define DEBUG_SORT 0
#define DEBUG_MERGE 0

#ifdef DEBUG
    template<typename T>
void printArrayDebug(T arr[],int n,int start=0) // DEBUG
{
    if (!DEBUG) return;
    //int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0;i<n;++i)
    {
        std::cout<<std::setw(4)<<arr[i+start]<<" ";
    }
    std::cout<<std::endl;
}

void printPartition(int InLength, int start, int size,const std::string& prefix=""){
//  ._.
//  | 
//   - 
    std::string line1=prefix+"+",
                line2=prefix+"|",
                line3=prefix+"+";

    for(int i=0;i<InLength;++i)
    {
        line1+="-";
        line3+="-";
        if (i>=start and i< start+size){
            line2 += "x";
        } else {
            line2 += " ";
        }
            
        if (i != InLength-1)    {
            line1+="+";
            line2+=" ";
            line3+="+";
        }
    }

    line1+="+";
    line2+="|";
    line3+="+";

    for (int i= InLength;i< start+size ;++i) {
        line2 += "x ";
    }
    std::cout<<line1<<std::endl;
    std::cout<<line2<<std::endl;
    std::cout<<line3<<std::endl;
    if (start+size > InLength){std::cout<<"ERROR HERE"<<std::endl;}
}

#endif


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


int get_pow2_size(unsigned int n){
    if (n && !(n&(n-1)))return n;
    int count=0;
    while(n) {n>>=1;++count;}
    return 1<<count;
}

unsigned int PowerOf2LessThan(unsigned int n){
    unsigned int i=1;
    unsigned int prev=1;
    while (i<n){
        i<<=1;
        if (i<n) {
            prev =i;
        } else {
            return prev;
        }
    }
}


//unsigned LatencyOfBitonicMerge( unsigned Size ) {
//    //  int(ceil(log2(i)));  
//    //  following vhdl code
//    unsigned Merge1Latency =0 , Merge2Latency= 0;
//    if (Size <= 1) return 0;
//
//      Merge1Latency = LatencyOfBitonicMerge( PowerOf2LessThan( Size ) );
//      Merge2Latency = LatencyOfBitonicMerge( Size - PowerOf2LessThan( Size ) );
//
//      return 1 + max( Merge1Latency , Merge2Latency );
//}
//
//unsigned int LatencyOfBitonicSort( unsigned int InSize  , unsigned int OutSize  ) 
//{
//    unsigned  Sort1Size =0 , Sort2Size    =0;
//    unsigned  Sort1Latency =0 , Sort2Latency= 0;
//    unsigned  MergeLatency               = 0;
//
//    if (InSize <= 1) return 0;
//
//      Sort1Size    = InSize / 2;
//      Sort2Size    = InSize - Sort1Size;
//
//      Sort1Latency = LatencyOfBitonicSort( Sort1Size , OutSize );
//      Sort2Latency = LatencyOfBitonicSort( Sort2Size , OutSize );
//      MergeLatency = LatencyOfBitonicMerge( min( Sort1Size , OutSize ) + min( Sort2Size , OutSize ) );
//
//      return max( Sort1Latency , Sort2Latency ) + MergeLatency;
//}



template<typename T>
void bitonicMerge(T in[], int InSize,T out[],int OutSize, bool dir,const std::string&debug="")
{
    //printDebug;
    if(DEBUG_MERGE)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"<<" Begin "
        <<" InSize="<<InSize
            <<" OutSize="<<OutSize
            <<" dir="<< dir << ((dir)?"(ascending)":"(descending)")
            <<std::endl;
    if(DEBUG_MERGE) std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @in in="; printArrayDebug(in,InSize) ;


    // size == 1 -> pass through
    if (InSize<=1) { 
        for(int i=0;i<min(InSize,OutSize);++i) out[i]=in[i]; 
        //printEndDebug;
        return;
    }

    if (InSize>1){
        int LowerSize  = PowerOf2LessThan( InSize ); //-- LowerSize >= Size / 2
        int UpperSize   =  InSize - LowerSize; //-- UpperSize < LowerSiz   

        if (LowerSize < UpperSize) std::cout<<"[ERROR]"<<__FUNCTION__<<" LowerSize ("<<LowerSize<<") not > of UpperSize ("<<UpperSize<<")"<<std::endl;

        if(DEBUG_MERGE)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"
            <<" LowerSize="<<LowerSize
                <<" UpperSize="<<UpperSize
                <<std::endl;

        //G21 : FOR x IN 0 TO UpperSize-1 GENERATE

        for (int i =0 ; i< UpperSize;++i) {
            if(in[i] > in[i+LowerSize] == dir) 
            {
                // this checks should refer to the comments, "just needs to be long enough"
                if( i<OutSize) out[i] = in[i+LowerSize];
                if(i+LowerSize<OutSize) out[i+LowerSize] = in[i];
            }
            else
            {
                if(i<OutSize) out[i] = in[i];
                if(i+LowerSize<OutSize) out[i+LowerSize] = in[i+LowerSize];
            }
        }   

        if (LowerSize > UpperSize){
            for(int i= UpperSize;i<LowerSize/*-1*/; ++i)
            {
                if (i<OutSize) out[i] = in[i];
            }
        }

        //void bitonicMerge(T in[], int InSize,T out[],int OutSize, bool dir)
        T out2[LowerSize];
        bitonicMerge(out,LowerSize,out2,LowerSize,dir ,debug + "   ");

        // I1( 0 TO Size - LowerSize-1 ) <= T1( LowerSize TO Size-1 );
        T out3[UpperSize];
        bitonicMerge(out+LowerSize, UpperSize, out3,UpperSize,dir,debug+"   "); // not dir? TODO check 

        //G23 : FOR x IN PipeLength DOWNTO 1 GENERATE
        //      T3( x ) <= T3( x-1 ) WHEN RISING_EDGE( clk );
        //    END GENERATE G23;
        //    T2( 0 TO LowerOutSize-1 ) <= T1( UpperSortLatency-LowerSortLatency )( 0 TO LowerOutSize-1 );
        //    I1( 0 TO InSize - LowerInSize-1 ) <= VectorIn( LowerInSize TO InSize-1 );
        //    T2( LowerOutSize TO LowerOutSize + UpperOutSize-1 ) <= O1( 0 TO UpperOutSize-1 );

        //std::cout<<"[DEBUG]  in was: ";printArrayDebug(in,InSize);

        // copy back to out; direction dependent.
        if (dir) // ascending -- Copy up to OutSize
        {
            for(int i=0;i<OutSize;++i) {
                if(i<UpperSize) out[OutSize-i-1] = out3[UpperSize-i-1];
                else out[OutSize-i-1] = out2[LowerSize-i-1+UpperSize];
            }

            //std::cout<<"[DEBUG] Merge Copy"<<dir<<"(ascending) "<<std::endl; 
            //std::cout<<"   - out2: ";printArrayDebug(out2,LowerSize);
            //std::cout<<"   - out3: ";printArrayDebug(out3,UpperSize);
            //std::cout<<"   -> out: ";printArrayDebug(out,OutSize);
        }
        else{ //descending
            for(int i=0;i<LowerSize;++i) {if(i<OutSize) out[i] =out2[i];}
            for(int i=LowerSize;i<OutSize;++i) out[i] = out3[i-LowerSize];

            //std::cout<<"[DEBUG] Merge Copy "<<dir<<"(descending) "<<std::endl; 
            //std::cout<<"   - out2: ";printArrayDebug(out2,LowerSize);
            //std::cout<<"   - out3: ";printArrayDebug(out3,UpperSize);
            //std::cout<<"   -> out: ";printArrayDebug(out,OutSize);
        }

    }// InSize>1

#ifdef DEBUG
    if (DEBUG_MERGE>0) {
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end in="; printArrayDebug(in,InSize) ;
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end out="; printArrayDebug(out,OutSize) ;
    }
#endif
    //printEndDebug;

} // bitonicMerge

template<typename T>
void bitonicSort(T in[],int Start, int InSize,T out[], int OutSize, bool dir , const std::string&debug="")
{
#ifdef DEBUG
    //printDebug;

    if(DEBUG_SORT)std::cout<<debug << "[DEBUG]["<<__FUNCTION__<<"]"
        <<" Start="<<Start
            <<" InSize="<<InSize
            <<" OutSize="<<OutSize
            <<" dir="<<dir
            <<std::endl;

    if (DEBUG_SORT>0) {
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @in in="; printArrayDebug(in,InSize,Start) ;
    }
#endif


    if (InSize <= 1)
    {
        for(int i=0;i<min(InSize,OutSize);++i) out[i] = in[i+Start]; 
        return ;
    }

    int  LowerInSize = InSize / 2;
    int  UpperInSize = InSize - LowerInSize; //-- UpperSize >= LowerSize

    int  LowerOutSize = min( OutSize , LowerInSize );
    int  UpperOutSize = min( OutSize , UpperInSize );


    //int LowerSortLatency = LatencyOfBitonicSort( LowerInSize , LowerOutSize );
    //int UpperSortLatency = LatencyOfBitonicSort( UpperInSize , UpperOutSize );
    //
    //int PipeLength       = max( UpperSortLatency-LowerSortLatency , 2 );

    //if(DEBUG)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"
    //	<<" LowerInSize="<<LowerInSize
    //	<<" UpperInSize="<<UpperInSize
    //	<<" LowerOutSize="<<LowerOutSize
    //	<<" UpperOutSize="<<UpperOutSize
    //	<<std::endl;


    // sorted output
    T OutTmp[LowerOutSize+UpperOutSize];

    // sort first half
    bitonicSort(in,Start,LowerInSize,OutTmp,LowerOutSize,not dir ,debug+"  ");
    // -- Just needs to be long enough - anything unused will be synthesized away....
    //     G21 : FOR x IN PipeLength DOWNTO 1 GENERATE
    //       T1( x ) <= T1( x-1 ) WHEN RISING_EDGE( clk );
    //     END GENERATE G21;

#ifdef DEBUG 
    if (OutSize <1) {
        std::cout<<debug<<"[ERROR]: @mid2 Called "<<__FUNCTION__<<" with OutSize="<<OutSize<<std::endl;
        return;
    }

    if (DEBUG_SORT>0)
        std::cout<<debug<<"[DEBUG]: @mid2 "<<__FUNCTION__
            <<" Calling BS with:" 
            <<" Start (start+lowerInSize) = "<<Start+LowerInSize
            <<" Size (UpperInSize) = "<<UpperInSize
            <<" ShiftOutput (LowerOutSize) ="<<LowerOutSize
            <<" OutSize (UpperOutSize) ="<<UpperOutSize
            <<" original OutSize="<<OutSize
            <<" original InSize="<<InSize
            <<" original dir="<<dir
            <<std::endl;
    // relative
    //if (DEBUG_SORT>0) printPartition(Start+InSize, Start+LowerInSize, UpperInSize,"[DEBUG][INPUT]: ");
    //abs == N
    //if (DEBUG_SORT>0) printPartition(50, Start+LowerInSize, UpperInSize,"[DEBUG][INPUT]: ");
    //if (DEBUG_SORT>0) printPartition(LowerOutSize+UpperOutSize, LowerOutSize, UpperOutSize,"[DEBUG][OUTPUT]: ");

#endif
    // sort second half
    bitonicSort(in,Start+LowerInSize,UpperInSize,OutTmp+LowerOutSize,UpperOutSize,dir ,debug + "  ");

#ifdef DEBUG
    if (OutSize <1) {
        std::cout<<debug<<"[ERROR]: @mid Called "<<__FUNCTION__<<" with OutSize="<<OutSize<<std::endl;
        return;
    }

    if (DEBUG_SORT>0) {
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @mid in="; printArrayDebug(in,InSize,Start) ;
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @mid tmp="; printArrayDebug(OutTmp,LowerOutSize+UpperOutSize) ;
    }

    if(DEBUG_SORT)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @mid (before merge)"
        <<" InSize="<<InSize
            <<" OutSize="<<OutSize
            <<" LowerInSize="<<LowerInSize
            <<" UpperInSize="<<UpperInSize
            <<" LowerOutSize="<<LowerOutSize
            <<" UpperOutSize="<<UpperOutSize
            <<" dir="<<dir
            <<std::endl;

    // merge
    if(DEBUG_SORT)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"<<" Calling bitonicMerge from BS  with InSize="<< LowerOutSize+UpperOutSize <<" OutSize="<< OutSize<<std::endl;

#endif
   
    //bitonicMerge(OutTmp,LowerOutSize+UpperOutSize,out,OutSize, dir);
    int OutSize2= LowerOutSize+UpperOutSize;
    T outTmp2[OutSize2]; // porta sfiga se OutSize<OutSize2
    bitonicMerge(OutTmp,LowerOutSize+UpperOutSize,outTmp2,OutSize2, dir);
    //copy back to out the first OutSize
    for(int i=0;i<OutSize;++i){
        if (dir) { //ascending
            out[OutSize-1-i] = outTmp2[OutSize2-1-i];
        }else{ //descending
            out[i] =outTmp2[i];
        }
    }


#ifdef DEBUG
    if (DEBUG_SORT>0) {
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end in="; printArrayDebug(in,InSize,Start) ;
        std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end out="; printArrayDebug(out,OutSize) ;
    }
#endif
    //printEndDebug;
}

    template<typename T>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[], T out[]) 
{  // just an interface
    printDebug;
    bitonicSort(in,0, nIn,out, nOut,0); 
    printEndDebug;
}
#endif
