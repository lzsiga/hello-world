#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

typedef struct StatData {
    int subset_number;
} StatData;

// Function to print a combination
static void printCombination(int subset[], int r, int fn_type, int subset_number) {
    if (!fn_type) printf("0_rec:>%d:{",subset_number); else printf("1_loop>%d:{", subset_number);
    for (int i = 0; i < r; i++)
        if(i<r-1)
               printf("%d,", subset[i]);
        else
               printf("%d", subset[i]);
    printf("}\n");
}

// Recursive function to generate combinations
// arr/arrv  set of elements                              //n  number of set elements                     //r  number of subset elements
// index     process set elements, starting from 'index'  //k  already generated subset elements (k<=r)
// subset    buffer for subset elements
void generateCombinationsRecursive(int arr[], int n, int r, int index, int k, int *subset, StatData *sdp) {
    if (k == r) {
        ++sdp->subset_number;
        printCombination(subset, r, 0, sdp->subset_number);
        return;
    }
    for (int i = index; i < n; i++) {
        subset[k] = arr[i];
        generateCombinationsRecursive(arr, n, r, i + 1, k + 1, subset, sdp);
    }
}

// Wrapper Function for Recursion
void runRecursive(int arr[], int n, int r, StatData *sdp) {
    int subset[r];
    generateCombinationsRecursive(arr, n, r, 0, 0, subset, sdp);
}

// Main Function for Execution
int main() {
    int arr[] = {1, 2, 3, 4, 12, 9, 5, 70}; // Example array
    int arrc = sizeof(arr) / sizeof(arr[0]);
    int r = (arrc+2)/3;
    StatData sd;

    clock_t start, end;
    double time_rec;

    memset(&sd, 0, sizeof sd);
    // Measure Time for Recursive Approach
    start = clock();
    runRecursive(arr, arrc, r, &sd);
    end = clock();
    time_rec = ((double)(end - start)) / CLOCKS_PER_SEC;


    // Print Results
    //printf("Number of Combinations (Recursive): %d\n", count_rec);
    printf("Execution Time (Recursive): %f seconds\n", time_rec);

    return 0;
}
