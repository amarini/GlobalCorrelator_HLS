/* DEBUG
#include <iostream>
#include <iomanip>      // std::setw
*/

#ifndef AVL_H
#define AVL_H


/*
template<typename T>
void printArray(T arr[],int n) // DEBUG
{
    //int n = sizeof(arr)/sizeof(arr[0]);
    for(int i=0;i<n;++i)
    {
        std::cout<<std::setw(4)<<arr[i]<<" ";
    }
    std::cout<<std::endl;
}
*/

inline int max(int a, int b)  
{  
    return (a > b)? a : b;  
}  
inline int balance( int y , int left[], int right[], int height[])
{
    //if (N == NULL)  
    //    return 0;  
    int hly = (left[y] >=0) ? height[left[y]]:-1;
    int hry = (right[y] >=0) ? height[right[y]]:-1;
    return hly - hry;  
}

inline void update_height( int y, int left[], int right[], int height[])
{
    int hly = (left[y] >=0) ? height[left[y]]:-1;
    int hry = (right[y] >=0) ? height[right[y]]:-1;
    height[y] = max(hly,hry)+1;
}

// helper functions
template<typename T>
int left_rotate( int x,T arr[], int left[], int right[], int parent[],int height[],int n)
{
    int y = right[x];
    int t2 = left[y];

    //int hly = (left[y] >=0) ? height[left[y]]: -1;  // T2
    //int hry = (right[y] >=0) ? height[right[y]]:-1;
    //int hlx = (left[x] >=0) ? height[left[x]]: -1;
    //int hrx = (right[x] >=0) ? height[right[x]]:-1;

  
    // Perform rotation  
    left[y] = x;
    right[x] = t2;

    //fix parent linking and height?
    int px=parent[x];
    if (px>=0 and left[px] == x ) left[px] =y;
    if (px>=0 and right[px] == x ) right[px] =y;

    parent[y] = parent[x];
    parent[x] = y;
    if (t2>=0) parent[t2] = x;
  
    // Update heights  

    update_height( x, left, right, height);
    update_height( y, left, right, height);

    // Return new root  
    return y;    
}

template<typename T>
int right_rotate( int y,T arr[], int left[], int right[],int parent[], int height[],int n)
{
    int x = left[y]  ;
    int t2 = right[x];

    //int hly = (left[y] >=0) ? height[left[y]]: -1;
    //int hry = (right[y] >=0) ? height[right[y]]:-1;
    //int hlx = (left[x] >=0) ? height[left[x]]: -1;
    //int hrx = (right[x] >=0) ? height[right[x]]:-1;

    // Perform rotation  
    right[x] = y;
    left[y] = t2;
    if (t2 >=0 ) parent[t2] = y;

    //fix parent linking and height?
    int py=parent[y];
    if (py>=0 and left[py] == y ) left[py] =x;
    if (py>=0 and right[py] == y ) right[py] =x;

    parent[x] = parent[y];
    parent[y] = x;

    // Update heights  
    update_height( y, left, right, height);
    update_height( x, left, right, height);
    
    return x;
}

/*
//DEBUG
inline int is_tree_consistent(int root,int n, int left[], int right[], int parent[],int height[])
{
    if (parent[root] >=0) {std::cout <<"[ERROR]"<<" tree inconsistent: root "<<root<<" parent "<<parent[root]<<std::endl;}
    for (int i=0 ;i<n;++i) 
    {
        if (i!=root and parent[i]<0) { std::cout<<"[ERROR]"<<" double root found: (root) "<<root<<" and node "<<i<<std::endl ;}
        if (parent[i] >=0 ){ // check parent-child relations of node i
            if (left[parent[i]] == i  ) // left child
            {}
            else if (right[parent[i]]==i) // right child
            {}
            else // link broken
            {
                std::cout<<"[ERROR]"<<" node "<<i<<" sees as parent "<<parent[i]<<" but parent, sees as childrens "<<left[parent[i]]<< " "<<right[parent[i]]<<std::endl;
            }
        }  
        // opposite, checks sons
        if (left[i] >=0 and parent[left[i]] != i) 
                std::cout<<"[ERROR]"<<" node "<<i<<" sees as left children "<<left[i]<<" but left child, sees as parent "<<parent[left[i]]<< " "<<std::endl;
        if (right[i] >=0 and parent[right[i]] != i) 
                std::cout<<"[ERROR]"<<" node "<<i<<" sees as right children "<<right[i]<<" but right child, sees as parent "<<parent[right[i]]<< " "<<std::endl;
        // checks heights
        int hl = (left[i] >=0) ? height[left[i]]:-1;
        int hr = (right[i] >=0) ? height[right[i]]:-1;
        if (height[i] != max(hl,hr)+1)
            std::cout<<"[ERROR]"<<" inconsistent height at node "<<i<<": h="<<height[i]<<"; l="<<hl<<" r="<<hr<<std::endl;
    }
    return 0 ;
}
*/


template<typename T> 
int insert( int n,int root, T arr[], int left[], int right[],int parent[], int height[])
{
    // the nth element is put in the tree structure
    left[n] = -1;
    right[n] = -1;
    height[n] = 0;
    parent[n] = -1;
    T key = arr[n];
    
    /*
    std::cout<<"----"<<__FUNCTION__<<"---- init----------"<<std::endl;
    std::cout<<"n="<<n<<"  ; root="<<root<<std::endl;
    int numbers[n+1]; for(int i=0;i<n+1;++i) numbers[i] = i;
    std::cout<<"index | ";printArray(numbers,n+1); 
    std::cout<<"array | ";printArray(arr,n+1); 
    std::cout<<"left  | ";printArray(left,n+1); 
    std::cout<<"right | ";printArray(right,n+1); 
    std::cout<<"height| ";printArray(height,n+1); 
    std::cout<<"parent| ";printArray(parent,n+1); 
    std::cout<<"------------------------------------"<<std::endl;
    */

    if (root==n) return root;

    int pos=root;
    while (true) 
    {
        if (key < arr[pos]) {
            int l =left[pos];
            if ( ((right[pos]>=0)?height[right[pos]]:-1) <= ( (left[pos]>=0) ?height[left[pos]]:-1 ))height[pos] +=1;
            if (l <0){ // NULL 
                //return l;
                left[pos] = n;
                parent[n] = pos;
                break;
            }
            pos = l;
            }
        else //if (arr[pos]< key) <=
            {
            if ( ((left[pos]>=0)?height[left[pos]]:-1) <= ( (right[pos]>=0) ?height[right[pos]]:-1 ))height[pos] +=1;
            int r = right[pos];
            if (r<0) {
                right[pos] = n;
                parent[n] = pos;
                //return r ;
                break;
                }
            pos = r;
            }
    } // finished BST insertion

    /*
    std::cout<<"----"<<__FUNCTION__<<"---- after bst insertion----------"<<std::endl;
    std::cout<<"n="<<n<<"  ; root="<<root<<std::endl;
    std::cout<<"index | ";printArray(numbers,n+1); 
    std::cout<<"array | ";printArray(arr,n+1); 
    std::cout<<"left  | ";printArray(left,n+1); 
    std::cout<<"right | ";printArray(right,n+1); 
    std::cout<<"height| ";printArray(height,n+1); 
    std::cout<<"parent| ";printArray(parent,n+1); 
    std::cout<<"------------------------------------"<<std::endl;

    is_tree_consistent(root, n, left, right,parent,height);
    */

    // balancing
    pos=n;

    while(pos>=0)
    {

        int b = balance(pos, left, right, height); 
        //std::cout<<"----"<<"pos="<<pos<<" b="<<b<<std::endl;
        // Left Left Case  
        if (b > 1 and key < arr [ left[pos] ])  
        {
            //std::cout<<"----"<<"LL (right-rotate "<<pos<<")"<<std::endl;
            right_rotate(pos, arr, left, right,parent, height,n);
        }
        // Right Right Case  
        else if (b < -1 and arr[right[pos]] < key)  
        {
            //std::cout<<"----"<<"RR (left-rotate "<<pos<<")"<<std::endl;
            left_rotate(pos,arr, left, right,parent, height,n);  
        }
        // Left Right Case  
        else if (b > 1 and arr[left[pos]] <key)  
        {  
            //std::cout<<"----"<<"LR (left-rotate "<<left[pos]<<" + right-rotate "<<pos<<")"<<std::endl;
            left[pos] = left_rotate(left[pos],arr, left, right,parent, height,n);
            parent[left[pos]] = pos;
            right_rotate(pos,arr, left, right,parent, height,n);
        }  
        // Right Left Case  
        else if (b < -1 and key < arr[right[pos]])  
        {  
            //std::cout<<"----"<<"RL (right-rotate"<<right[pos]<<" + left-rotate "<<pos<<")"<<std::endl;
            right[pos] = right_rotate(right[pos],arr, left, right,parent, height,n);
            parent[right[pos]] = pos;
            left_rotate(pos,arr, left, right,parent, height,n);
        }  

        update_height( pos, left, right, height);
        if (parent[pos]<0) {
            //is_tree_consistent(pos, n, left, right,parent,height);
            return pos; // return root
        }
        pos=parent[pos];
    }   


    return -1;
}

inline int next(int x, int left[], int right[], int parent[])
{

    if (right[x] >=0 ) { // if I have a right child, go there and decend left
        x= right[x];
        while (left[x]>=0) x=left[x];
        return x;
        }
    // go up until I'm a left child, go up of 1 and return
    if (parent[x] >=0 and left[parent[x]] == x) return parent[x];
    //while (parent[x] >=0 and left[parent[x]] == x) x=parent[x];

    // go up until I'm a right child
    while (parent[x] >=0){
      if (right[parent[x]] == x) x= parent[x];
      else return parent[x]; // left child
    }

    return -1;
}

template<typename T>
void partial_avl_sort(T arr[], int n, int m)
{
    // 3 set of indexes.
    // cannot implemnt them as std array representation. I need navigation for lr-rotate
    int left[n];
    int right[n];
    int height[n];
    int parent[n];
    int root=0;
   
    // construct avl tree 
    for(int i=0;i<n;++i) root =insert( i, root, arr, left, right, parent, height);

    int i=0; // [,i]  is sorted
    int pos=root; 

#ifdef NPARTICLES
    T sorted[NPARTICLES]; // cannot have it variable length in fpga?
#else
    T sorted[n]; //  for debug in c++
#endif
    
    while (left[pos]>=0) // logN
    {
       pos = left[pos];
    }

    while ( pos>=0) 
    {
        sorted[i] = arr[pos]; 
        i++; 
        pos=next(pos,left,right,parent);    
    }
    //copy back
    for (int i=0;i<n;++i) arr[i] = sorted[i];
    return; 
}

#endif
