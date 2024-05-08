#pragma once 

template <typename T>
void insertionSort(T* array, int left, int right) {
    int i = left + 1;

    while (i <= right) {
        int j = i;
        while (j > left && array[j] < array[j-1]) {
            T tmp = array[j];
            array[j] = array[j-1];
            array[j-1] = tmp;
            j--;
        }
        i++;
    }   

}