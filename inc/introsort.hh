#pragma once 

#include <cmath>
#include "quicksort.hh"
#include "heapsort.hh"
#include "insertionsort.hh"

template <typename T>
void introSortMain(T* array, int left, int right, int depthLimit) {
    if (right-left < 9) {
        insertionSort(array,left,right);
        return;
    }
    if (depthLimit == 0 && right - left > 1) {
        heapSort(array,left,right);
        return;
    }
    if ( left < right) {   
        int last = partition(array,left,right);
        introSortMain(array, left, last, depthLimit - 1);
        introSortMain(array, last+1, right, depthLimit - 1);
    }   
}

template<typename T>
void introSort(T* array, int left, int right) {

int depthLimit = 2*log(right-left);

introSortMain(array, left, right, depthLimit);
}