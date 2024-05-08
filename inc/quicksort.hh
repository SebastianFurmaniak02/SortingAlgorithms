#pragma once
#include "random.hh"

template <typename T>
int partition(T* array, int left, int right) {
    T temp, pivot = array[(left+right)/2]; 
    int l = left, r = right;
    while (1) {
        while (array[l] < pivot) l++;
        while (array[r] > pivot) r--;

        if (l >= r)
        return r;
        temp = array[l];
        array[l++] = array[r];
        array[r--] = temp;
    }
}

template <typename T>
void quickSort (T* array, int left, int right) {
    if (left < right) {
        int last = partition(array,left,right);
        quickSort(array, left, last);
        quickSort(array, last+1, right);    
    }
}