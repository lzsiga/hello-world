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
     if (!fn_type)
         printf("\t0_rec:>%d:{",subset_number);
     else
         printf("1_loop>%d:{", subset_number);
     for (int i = 0; i < r; i++)
         if(i<r-1)
             printf("%d,", subset[i]);
         else
             printf("%d", subset[i]);
     printf("}\n");
     fflush(stdout);
}

// Recursive function to generate combinations
// arr/arrv  set of elements
// n  number of set elements
// r  number of subset elements
// index process set elements, starting from 'index'
// k  already generated subset elements (k<=r)
// subset buffer for subset elements
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

// Brute Force (Loop-Based) Approach
int runLoopBased(int arr[], int n, int r) {
    assert(r==3);
    int subset[r];
    int subset_number=0;
    for (int i = 0; i < n - 2; i++) {
        subset[0] = arr[i];
        for (int j = i + 1; j < n - 1; j++) {
            subset[1] = arr[j];
            for (int k = j + 1; k < n; k++) {
                subset[2] = arr[k];
                printCombination(subset, r, 1, ++subset_number);
            }
        }
    }
    return subset_number;
}

static void measureRec(int arr[], int arrc, int r)
{
    StatData sd;
    memset(&sd, 0, sizeof sd);

    for (int i=0; i<5e4; ++i) runRecursive(arr, arrc, r, &sd);
}

static void measureLoop(int arr[], int arrc, int r)
{
    for (int i=0; i<5e4; ++i) runLoopBased(arr, arrc, r);
}

// Main Function for Execution
int main() {
   int arr[] = {1, 2, 3, 4, 12, 9, 5, 70};
   int arrc = sizeof(arr) / sizeof(arr[0]);
   int r = (arrc+2)/3;
   clock_t start, end;
   double time_rec, time_loop;

    // Measure Time for Recursive Approach
    start = clock();
    measureRec(arr, arrc, r);
    end = clock();
    time_rec = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure Time for Loop-Based Approach
    start = clock();
    measureLoop(arr, arrc, r);
    end = clock();
    time_loop = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print Results
    fprintf(stderr, "Execution Time (Recursive): %f seconds\n", time_rec);
    fprintf(stderr, "Execution Time (Loop-Based): %f seconds\n", time_loop);

    return 0;
}
