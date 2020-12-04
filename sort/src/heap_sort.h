// from: https://www.geeksforgeeks.org/heap-sort/
// from: https://www.geeksforgeeks.org/iterative-heap-sort/
// modified by: Andrea Carlo Marini
//  - templated
//  - stop after m iteration
//  - include pragma for hls studies
template<typename T>
void myswap(T& a, T&b)
{
	T c = a;
	a=b;
	b=c;
	return;
}

/*
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
template<typename T>
void heapify(T arr[], int n, int i)
{
    #pragma HLS ARRAY_PARTITION variable=arr complete
    #pragma HLS pipeline II=1
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n and arr[l] > arr[largest])
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n and arr[r] > arr[largest])
        largest = r;
 
    // If largest is not root
    if (largest != i) {
        myswap(arr[i], arr[largest]);
 
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}
*/

// function build Max Heap where value  
// of each child is always smaller 
// than value of their parent . Iterative ,non Recursive
template<typename T>
void buildMaxHeap(T arr[], int n)  
{  
    #pragma HLS ARRAY_PARTITION variable=arr complete

    for (int i = 1; i < n; i++)  
    { 
        #pragma HLS pipeline II=1
        // if child is bigger than parent 
        //if (arr[i] > arr[(i - 1) / 2])  
        if (arr[(i - 1) / 2] < arr[i])  
        { 
            int j = i; 
      
            // swap child and parent until 
            // parent is smaller 
            //while (arr[j] > arr[(j - 1) / 2])  
            while (arr[(j - 1) / 2] < arr[j])  
            { 
                myswap(arr[j], arr[(j - 1) / 2]); 
                j = (j - 1) / 2; 
            } 
        } 
    } 
} 


 
// main function to do heap sort
/* Construct an array 'a' of n elements with m sorted elements at the end
 * a = input array
 * n = size of the input array
 * m = number of sorted elements
 */
template <typename T>
void partial_heap_sort(T arr[], int n, int m)
{
#pragma HLS ARRAY_PARTITION variable=arr complete
#pragma HLS pipeline II=1
    // Build heap (rearrange array)
    buildMaxHeap(arr, n); 

    // One by one extract an element from heap
    //for (int i = n - 1; i > 0; i--) 
    for (int i = n - 1; i > n-1-m and i>0; --i) { // the additional condition is because the last round is with 1 element
#pragma HLS pipeline II=1
        // Move current root to end
        myswap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        //heapify(arr, i, 0);

        // maintaining heap property 
        // after each swapping 
        int j = 0, index; 
        do
        { 
            index = (2 * j + 1); 

            // if left child is smaller than  
            // right child point index variable  
            // to right child 
            if (arr[index] < arr[index + 1] and index < (i - 1)) index++; 

            // if parent is smaller than child  
            // then swapping parent with child  
            // having higher value 
            if (arr[j] < arr[index] && index < i) myswap(arr[j], arr[index]); 

            j = index; 

        } while (index < i); 
    } 
}
 
 
