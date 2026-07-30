#include "c1_mergeSort.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
