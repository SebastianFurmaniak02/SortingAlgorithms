#pragma once

template <typename T>
void swap(T* array, int i1, int i2) {
    if (i1 != i2) {
        T tmp = array[i1];
        array[i1] = array[i2];
        array[i2] = tmp;
    }
}

template <typename T>
void heapify(T *array, int size, int root) {
    int largest = root, l = 2*root+1, r = 2*root+2;
    if (l < size  && array[l] > array[largest]) largest = l;
    if (r < size  && array[r] > array[largest]) largest = r;
    if (largest != root) {
        swap(array,root,largest);
        heapify(array,size,largest);
    }
}

template <typename T>
void heapSort(T *array, int left, int right) {
    T * tmp = &array[left];
    int lastIndex = right - left, size = right - left + 1;

    for (int i = size/2; i >= 0; i--) heapify(tmp,size,i);
    for (int i = lastIndex; i > 0; i--) {
        swap(tmp, 0, i);
        size--;
        heapify(tmp,size,0);
    } 
}