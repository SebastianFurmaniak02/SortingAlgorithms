#pragma once

template <typename T>
void merge(T* array, int left, int middle, int right) {
    T* tmp = new T[right - left + 1];
    int i = left, j = middle + 1, k = 0;

    while (i <= middle && j <= right) {
        if (array[j] < array[i]) {
            tmp[k++] = array[j++];
        }
        else {
            tmp[k++] = array[i++];
        }
    }

    if (i <= middle) {
        while (i <= middle) tmp[k++] = array[i++];
    }
    else {
        while (j <= right) tmp[k++] = array[j++];
    }

    for (k = 0; k <= right - left; k++) array[left + k] = tmp[k];

    delete[] tmp;
}

template <typename T>
void mergeSort (T* array, int left, int right) {
    if (left >= 0 && left < right) {
        int middle = (left+right)/2;
        mergeSort(array,left,middle);
        mergeSort(array,middle+1,right);
        merge(array, left, middle, right);
    }
}