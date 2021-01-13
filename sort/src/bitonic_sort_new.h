// define colors
#ifndef BITONIC_NEW_H
#define BITONIC_NEW_H

// master switch
#define DEBUG 1

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

	template<typename T>
void bitonicMerge(T in[], int InSize,T out[],int OutSize, bool dir,const std::string&debug="")
{
//	printDebug;
//	if(DEBUG)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"<<" Begin "
//		<<" InSize="<<InSize
//		<<" OutSize="<<OutSize
//		<<" dir="<<dir
//		<<std::endl;
//	if(DEBUG) std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @in in="; printArrayDebug(in,InSize) ;

	// size == 1 -> pass through
	if (InSize<=1) { 
		for(int i=0;i<min(InSize,OutSize);++i) out[i]=in[i]; 
		return;
		}
	if (InSize>1){
		int LowerSize  = PowerOf2LessThan( InSize ); //-- LowerSize >= Size / 2
		int UpperSize   =  InSize - LowerSize; //-- UpperSize < LowerSiz   G21 : FOR x IN 0 TO UpperSize-1 GENERATE

//	if(DEBUG)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"<<" Begin "
//		<<" LowerSize="<<LowerSize
//		<<" UpperSize="<<UpperSize
//		<<std::endl;

		for (int i =0 ; i< UpperSize;++i) {
			if(in[i] > in[i+LowerSize] == dir) 
			{
				out[i] = in[i+LowerSize];
				out[i+LowerSize] = in[i];
			}
			else
			{
				out[i] = in[i];
				out[i+LowerSize] = in[i+LowerSize];
			}
		}   
		
		if (LowerSize > UpperSize){
			for(int i= UpperSize;i<LowerSize/*-1*/; ++i)
			{
				out[i] = in[i];
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

		// copy back to out; direction dependent.
        if (dir) // ascending -- Copy up to OutSize
        {
            for(int i=0;i<OutSize;++i) {
                if(i<UpperSize) out[OutSize-i-1] = out3[UpperSize-i-1];
                else out[OutSize-i-1] = out2[LowerSize-i-1+UpperSize];
            }

            std::cout<<"[DEBUG] Merge Copy"<<dir<<"(ascending) "<<std::endl; 
            std::cout<<"   - out2: ";printArrayDebug(out2,LowerSize);
            std::cout<<"   - out3: ";printArrayDebug(out3,UpperSize);
            std::cout<<"   -> out: ";printArrayDebug(out,OutSize);
        }
        else{ //descending
            for(int i=0;i<LowerSize;++i) out[i] =out2[i];
            for(int i=LowerSize;i<OutSize;++i) out[i] = out3[i-LowerSize];

            std::cout<<"[DEBUG] Merge Copy "<<dir<<"(descending) "<<std::endl; 
            std::cout<<"   - out2: ";printArrayDebug(out2,LowerSize);
            std::cout<<"   - out3: ";printArrayDebug(out3,UpperSize);
            std::cout<<"   -> out: ";printArrayDebug(out,OutSize);
        }

	}// InSize>1

//	#ifdef DEBUG
//		if (DEBUG>0) {
//			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end in="; printArrayDebug(in,InSize) ;
//			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end out="; printArrayDebug(out,OutSize) ;
//		}
//	#endif
//	printEndDebug;

} // bitonicMerge

	template<typename T>
void bitonicSort(T in[],int Start, int InSize,T out[], int OutSize, bool dir , const std::string&debug="")
{

//	if(DEBUG)std::cout<<debug<<"[DEBUG] "<<__FUNCTION__<<" Begin"<<std::endl;
//	if(DEBUG)std::cout<<debug << "[DEBUG]["<<__FUNCTION__<<"]"
//		<<" Start="<<Start
//		<<" InSize="<<InSize
//		<<" OutSize="<<OutSize
//		<<" dir="<<dir
//		<<std::endl;
//
//	#ifdef DEBUG
//		if (DEBUG>0) {
//			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @in in="; printArrayDebug(in,InSize,Start) ;
//		}
//	#endif


	if (InSize <= 1)
	{
		for(int i=0;i<min(InSize,OutSize);++i) out[i] = in[i+Start]; 
		return ;
	}

	int  LowerInSize = InSize / 2;
	int  UpperInSize = InSize - LowerInSize; //-- UpperSize >= LowerSize

	int  LowerOutSize = min( OutSize , LowerInSize );
	int  UpperOutSize = min( OutSize , UpperInSize );

/*
	if(DEBUG)std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"]"
		<<" LowerInSize="<<LowerInSize
		<<" UpperInSize="<<UpperInSize
		<<" LowerOutSize="<<LowerOutSize
		<<" UpperOutSize="<<UpperOutSize
		<<std::endl;
*/

	// sorted output
	T OutTmp[LowerOutSize+UpperOutSize];
	// sort first half
	bitonicSort(in,Start,LowerInSize,OutTmp,LowerOutSize,dir ,debug+"  ");
	// sort second half
	bitonicSort(in,Start+LowerInSize,UpperInSize,OutTmp+LowerOutSize,UpperOutSize,not dir ,debug + "  ");

/*
	#ifdef DEBUG
		if (DEBUG>0) {
			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @mid in="; printArrayDebug(in,InSize,Start) ;
			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @mid tmp="; printArrayDebug(OutTmp,LowerOutSize+UpperOutSize) ;
		}
	#endif
*/

	// merge
	bitonicMerge(OutTmp,LowerOutSize+UpperOutSize,out,OutSize, dir);


//	#ifdef DEBUG
//		if (DEBUG>0) {
//			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end in="; printArrayDebug(in,InSize,Start) ;
//			std::cout<<debug<<"[DEBUG]["<<__FUNCTION__<<"] @end out="; printArrayDebug(out,OutSize) ;
//		}
//	#endif

}

	template<typename T>
void sort_and_crop_ref(unsigned int nIn, unsigned int nOut, T in[], T out[]) 
{  // just an interface
	//printDebug;
	bitonicSort(in,0, nIn,out, nOut,0); 
	//printEndDebug;
}
#endif
