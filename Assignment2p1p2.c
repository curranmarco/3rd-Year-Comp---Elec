#include <stdio.h>
#include <stdlib.h>
#include "t1.h"
#include <time.h>


//Fills the array with ascending, consecutive numbers starting from 0.
void fill_ascending(int *array, int size){
    for (int i = 0; i < size; i++){
        array[i] = i;
    }
    
}
//Fills the array with descending numbers starting from size - 1
void fill_descending(int *array, int size){
    for (int i = 0; i < size; i++){
        array[i] = size - 1 - i;
    }
}

//Fills the array with uniform numbers
void fill_uniform(int *array, int size){
    for (int i = 0; i < size; i++){
        array[i] = 5;
    }
    
}

//Fills the array with random numbers within 0 and size - 1 with duplicates
void fill_with_duplicates(int *array, int size){
    //this will seed our number generator based on the current time ensuring randomness each time it's run
    srand(time(NULL));
    for (int i = 0; i < size; i++){
        array[i] = rand() % size;
    }
    
}


//Fills the array with unique numbers between 0 and size - 1 in a shuffled order without duplicates
void fill_without_duplicates(int *array, int size){
    for (int i = 0; i < size; i++){
        array[i] = i;
    }

    //this seeds out number generator based on the current time used in the following loop
    srand(time(NULL));

    //loop to randomlly allocate the numbers by swapping them
    for (int i = size - 1; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    
}

void printArray(int* arr, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}



