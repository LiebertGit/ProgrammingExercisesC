#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// prototypes
void mergeSort(int arr[], int left, int right);
void merge(int arr[], int left, int mid, int right);
int test(int arr[], int size);

int main() {
    srand(time(0));

    // array size from 8 to 12
    int min_range = 8;
    int max_range = 12;
    int size_range = max_range - min_range + 1;
    int array_size = (rand() % size_range) + min_range;

    int rnumbers[array_size];

    // array entry range 0 to 99
    int min_entry = 0;
    int max_entry = 99;
    int entry_range = max_entry - min_entry + 1;

    // fill array
    for (int i = 0; i < array_size; i++) {
        rnumbers[i] = (rand() % entry_range) + min_entry;
    }

    printf("This program gives a randomized array of length between %d and %d with entries between %d and %d\n",
           min_range, max_range, min_entry, max_entry);

    // unsorted
    printf("unsorted: [");
    for (int i = 0; i < array_size; i++) {
        printf("%d", rnumbers[i]);
        if (i < array_size - 1) printf(",");
    }
    printf("]\n");

    // sort
    mergeSort(rnumbers, 0, array_size - 1);

    // sorted
    printf("sorted: [");
    for (int i = 0; i < array_size; i++) {
        printf("%d", rnumbers[i]);
        if (i < array_size - 1) printf(",");
    }
    printf("]\n");

    // test result
    if (test(rnumbers, array_size))
        printf("Array is sorted correctly\n");
    else
        printf("Array is NOT sorted\n");

    return 0;
}

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
        if (arr[l_index] < arr[r_index])
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