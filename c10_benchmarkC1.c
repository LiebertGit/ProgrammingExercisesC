#include "mergeSort.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (void) {
    srand(time(NULL));

    int sizes [] = {1000, 10000, 100000, 1000000};
    int numberOfTests = sizeof(sizes) / sizeof(sizes[0]);

    for(int i = 0; i < numberOfTests; i++){
       int arraySize = sizes[i]; 

       int *numbers = malloc(arraySize * sizeof(int));

       if (numbers == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
       }

       //Fill array with random numbers
       for (int j = 0; j < arraySize; j++){
            numbers[j] = rand();
       }

       // Start timer HERE
       clock_t start = clock();

       mergeSort(numbers, 0, arraySize - 1);

       clock_t end = clock();

       double elapsedTime = (double)(end - start) / CLOCKS_PER_SEC;
       printf("Size: %d Time: %.6f seconds\n",
       arraySize,
       elapsedTime);

       if (!test(numbers, arraySize)){
            printf("Sorting failed.\n");
            free(numbers);
            return 1;
       }
    }

    return 0;
}