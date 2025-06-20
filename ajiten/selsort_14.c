/* selsort_14.c */

#include <stdio.h>

void printArray(int arr[], int size){
    int i;
    for (i = 0; i < size; i++)
        printf(" %d", arr[i]);
}

void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n) {
    int temp, min_loc, temp_loc=0;
    // Start with the whole array as unsorted one, and sequentially move the unsorted subarray towards right
    for (int i = 0; i < n - 1; i++) {
        printf("\n====Pass no: %d, i: %d====================\n", i+1, i);
        //Find the minimum element in unsorted array
        min_loc = i;
        for (int j = i + 1; j < n; j++) {
            printf("\nj: %d :: arr[%d]=%d <<>> min_loc: %d :: arr[%d]=%d", j, j, arr[j], min_loc, min_loc, arr[min_loc]);
            if (arr[j] < arr[min_loc]) {
                temp_loc = min_loc;
                min_loc = j;
                printf("\n \t<<>>min_loc changed to: %d :: swap between loc: %d (=%d), & %d (=%d) => arr:", j, j, arr[j], i, arr[i]);
                swap(&arr[temp_loc], &arr[min_loc]);
                printArray(arr, n);
            }
        } 
     }
     printArray(arr, n); 
}

int main() {
    int arr[8] = { 5, 1, 3, 16, 8, 12, 11, 2 };//arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("Unsorted array: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
  
    // Calling selection sort
    selectionSort(arr, n);

    printf("\nSorted array: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
