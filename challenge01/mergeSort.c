#include "mergeSort.h"
#include <stdio.h>

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge(int arr[], int left, int mid, int right) {
    int temp[right - left + 1];

    int l_index = left;
    int r_index = mid + 1;
    int i = 0;

    while (l_index <= mid && r_index <= right) {
        if (arr[l_index] <= arr[r_index])
            temp[i++] = arr[l_index++];
        else
            temp[i++] = arr[r_index++];
    }

    while (l_index <= mid)
        temp[i++] = arr[l_index++];

    while (r_index <= right)
        temp[i++] = arr[r_index++];

    for (int j = left; j <= right; j++)
        arr[j] = temp[j - left];
}

int test(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            printf("Index %d is bigger than successor\n", i);
            return 0;
        }
    }
    printf("Sort test success\n");
    return 1;
}