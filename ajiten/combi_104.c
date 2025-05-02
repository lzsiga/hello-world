#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>


// Function to print a combination
static void printCombination(int subset[], int r, int fn_type, int subset_number) {
    if (!fn_type) printf("0_rec>%d:{", subset_number); else printf("1_loop>%d:{", subset_number);        //first version: using global-variables
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
void generateCombinationsRecursive(int arr[], int n, int r, int index, int k, int *subset, int *subset_number) {
    if (k == r) {
        ++*subset_number;
        printCombination(subset, r, 0, *subset_number);
        return;
    }
    for (int i = index; i < n; i++) {
        subset[k] = arr[i];
        generateCombinationsRecursive(arr, n, r, i + 1, k + 1, subset, subset_number);
    }
}

// Wrapper Function for Recursion
void runRecursive(int arr[], int n, int r) {
    int *subset = (int *)malloc(r * sizeof(int)); int subset_number = 0;
    generateCombinationsRecursive(arr, n, r, 0, 0, subset, &subset_number);
    free(subset);
}
// Brute Force (Loop-Based) Approach
int runLoopBased(int arr[], int n, int r) {
    assert(r==3);
    int *subset = (int *)malloc(r * sizeof(int));
    int subset_number=0, l=0;
    for (int i = 0; i < n - 2; i++) {
        subset[l] = arr[i];
        for (int j = i + 1; j < n - 1; j++) {
            subset[l+1] = arr[j];
            for (int k = j + 1; k < n; k++) {
                subset[l+1] = arr[k];
                printCombination(subset, r, 1, ++subset_number);
            }
        }
    }
    free(subset);
    return subset_number;
}

// Main Function for Execution
int main() {
    int arr[] = {1, 2, 3, 4, 12, 9, 5, 70}; // Example array
    int arrc = sizeof(arr) / sizeof(arr[0]);
    int r = (arrc+2)/3;

    clock_t start, end;
    double time_rec, time_loop;

    // Measure Time for Recursive Approach
    start = clock();
    runRecursive(arr, arrc, r);
    end = clock();
    time_rec = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure Time for Loop-Based Approach
    start = clock();
    runLoopBased(arr, arrc, r);
    end = clock();
    time_loop = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print Results
    printf("Execution Time (Recursive): %f seconds\n", time_rec);
    printf("Execution Time (Loop-Based): %f seconds\n", time_loop);

    printf("Ratio of the time taken by the Recursive version, to the time taken by the Loop based approach: %f times\n", time_rec/time_loop);


    return 0;
}
