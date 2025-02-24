#include <stdio.h>
#include "t2.h" // Assumes that swapFunction and partitions are defined in "t2.h"
#include "Assignment2p2.c"


void printArray(int array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    //test arrays 
    int array1[] = {29, 10, 14, 37, 13};
    int array2[] = {29, 10, 14, 37, 13};
    int array3[] = {29, 10, 14, 37, 13};
    int size = sizeof(array1) / sizeof(array1[0]);

    printf("Original Array:\n");
    printArray(array1, size);

    // Test Selection Sort
    number_comparisons = 0;
    number_swaps = 0;
    selectionSort(array1, size);
    printf("\nSelection Sort:\n");
    printArray(array1, size);
    printf("Comparisons: %d, Swaps: %d\n", number_comparisons, number_swaps);

    // Test Insertion Sort
    number_comparisons = 0;
    number_swaps = 0;
    insertionSort(array2, size);
    printf("\nInsertion Sort:\n");
    printArray(array2, size);
    printf("Comparisons: %d, Swaps: %d\n", number_comparisons, number_swaps);

    // Test Quick Sort
    number_comparisons = 0;
    number_swaps = 0;
    quickSort(array3, size);
    printf("\nQuick Sort:\n");
    printArray(array3, size);
    printf("Comparisons: %d, Swaps: %d\n", number_comparisons, number_swaps);

    return 0;
}
