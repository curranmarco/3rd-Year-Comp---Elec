#include <stdio.h> 
#include "t2.h"

int number_comparisons = 0;
int number_swaps = 0;


void selectionSort(int array[], int size) 
{ 
    //in this loop we are assuming the current element is the smallest or largest
  for (int i = 0; i < size; i++){
    int lowest = i;
    //here we are going to compare it to the smallest element found
    for (int temp1 = i + 1; temp1 < size; temp1++){
        //if a smaller one is found, update the index of the lowest value
        number_comparisons++;
        if (array[lowest] > array[temp1]){
            lowest = temp1; 
            number_swaps++;
        }
    }

    //here we are going to swap the elements if a smaller one is found
    if (lowest != i){
        //create a temp value to store the value of the fake lowest
        number_comparisons++;
        int temp2 = array[i];
        array[i] = array[lowest];
        array[lowest] = temp2;
        number_swaps++;
    }
  }
} 

void insertionSort(int array[], int size) { 
    //starting in the second element it will compare the element in the array with the previous elements in the left side 
    for (int i = 1; i < size; i++){
        int value = array[i];
        //setting temp to the last sorted element before value
        int temp = i - 1;
        number_comparisons++;

        //this loop finds the right space for the number by shiting the numbers that are larger to the right by swapping them
        while (temp >= 0 && array[temp] > value){
            array[temp + 1] = array[temp];
            number_comparisons++;
            number_swaps++;
            temp--;
        }
        
        //this is the new correct position of the last sorted value in our array
        array[temp + 1] = value;
    }
}

//function to swap values
void swapFunction( int *a, int *b){
    //temp variable to store a's value
    int temp1 = *a;
    *a = *b;
    *b = temp1;
    number_swaps++;
}

//Function to select pivot and move values to the left or the right of the pivot depending on if it's bigger or smaller
int partitions(int *array, int low, int high){
    //selecting our pivot which which is going to be the last element
    int pivotV = array[high];

    //the index of the smaller element
    int i = low - 1;

    for (int temp = low; temp < high; temp++){
        number_comparisons++;
        //here, if the element is less than the pivot element, we swap both 
        //this will move our smallest values to the left 
        if (array[temp] < pivotV){
            i++;
            swapFunction(&array[i], &array[temp]);
            
        }
    }

    //here, we swap the pivot with the element at i + 1 putting it between the small and large numbers
    swapFunction(&array[i + 1], &array[high]);
    
    //here we are returning th eindex of the next pivot for further sorting 
    return (i + 1);
}

//helper function to do the quick sorting correctly 
void qSHelper(int *array, int low, int high){
    if (low < high){
        //finding our starting index
        int index = partitions(array, low, high);

        //this will sort the left side array of the pivot
        qSHelper(array, low, index - 1);
        
        //and this does the right side 
        qSHelper(array, index + 1, high);
    }
}


void quickSort(int array[], int size) 
{ 
    qSHelper(array, 0, size - 1);
}


