#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

void swap(int *array, int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}


void printArray(int *array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void permute(int *array, int i, int length) { 
  if (length == i) {
     printArray(array, length);
     return;
  }
  int j = i;
  for (j = i; j < length; j++) { 
     swap(array, i , j);
     permute(array,i+1,length);
     swap(array, i , j);
  }
  return;
}
int main(int argc, char *argv[]) {

    int array[9] = {1,2,3,4,5,6,7,8,9};
    permute(array, 0, 9); 

    return 0;
}

