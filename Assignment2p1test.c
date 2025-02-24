#include "t1.h"
#include "Assignment2p1p2.c"
#include <stdio.h>



int main()
{
    int arraySize = 6;
    int array[arraySize];
    
    printf("%s\n", "Ascending: ");
    fill_without_duplicates(array, arraySize);
    printArray(array, arraySize);


    return 0;
}