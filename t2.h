#ifndef T2_H_
#define T2_H_
void selectionSort(int arr[], int size);
void insertionSort(int arr[], int size);
void quickSort(int arr[], int size);
void swapFunction( int *a, int *b);
int partitions(int *array, int low, int high);
void qSHelper(int *array, int low, int high);

extern int number_comparisons;
extern int number_swaps;


#endif