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
         printf("0_rec:>%d:{", subset_number);
     else
         printf("1_loop>%d:{", subset_number);

     for (int i = 0; i < r; i++) {
         if(i<r-1)
             printf("%d,", subset[i]);
         else
             printf("%d", subset[i]);
     }
     printf("}\n");
}

// Recursive function to generate combinations
// arr/arrv  set of elements
// n         number of set elements
// r         number of subset elements
// index     process set elements, starting from 'index'
// k         already generated subset elements (k<r)
// subset    buffer for subset elements
void generateCombinationsRecursive(int arr[], int n, int r, int index, int k, int *subset, StatData *sdp) {
    int lim= n-(r-k-1);

    for (int i = index; i < lim; i++) {
        subset[k] = arr[i];
        if (k+1==r) {
            ++sdp->subset_number;
            printCombination(subset, r, 0, sdp->subset_number);
        } else {
            generateCombinationsRecursive(arr, n, r, i + 1, k + 1, subset, sdp);
        }
    }
}

// Wrapper Function for Recursion
int runRecursive(int arr[], int n, int r, StatData *sdp) {
    int subset[r];
    memset(sdp, 0, sizeof *sdp);
    generateCombinationsRecursive(arr, n, r, 0, 0, subset, sdp);
    return sdp->subset_number;
}
// Brute Force (Loop-Based) Approach
int runLoopBased(int arr[], int n, int r) {
    assert(r==7);
    int subset[7];
    int subset_number=0;
    for (int i = 0; i < n - 6; i++) {
        subset[0] = arr[i];
        for (int j = i + 1; j < n - 5; j++) {
            subset[1] = arr[j];
            for (int k = j + 1; k < n - 4; k++) {
                subset[2] = arr[k];
                for (int l = k + 1; l < n - 3; l++) {
                    subset[3] = arr[l];
                    for (int m = l + 1; m < n - 2; m++) {
                        subset[4] = arr[m];
                        for (int o = m + 1; o < n - 1; o++) {
                            subset[5] = arr[o];
                            for (int p = o + 1; p < n; p++) {
                                subset[6] = arr[p];
                                ++subset_number;
                                printCombination(subset, r, 1, subset_number); //commented to allow correct comparison, of run-time   //line #79
                            }
                        }
                    }
                }
            }
        }
    }
    return subset_number;
}

// Main Function for Execution
int main() {
   int arr[] = {1, 2, 3, 4, 12, 9, 5, 70, 13, 19, 27, 7, 23, 26, 24, 11, 111, 67};
   int arrc = sizeof(arr) / sizeof(arr[0]);
   int r = 7;// (arrc+2)/3;

   int count_rec, count_loop;
   StatData sd;

   clock_t start, end;
   double time_rec, time_loop;

   memset(&sd, 0, sizeof sd);

   // Measure Time for Recursive Approach
   start = clock();
   for (int i=0; i<200; ++i) count_rec = runRecursive(arr, arrc, r, &sd);
   end = clock();
   time_rec = ((double)(end - start)) / CLOCKS_PER_SEC;

   // Measure Time for Loop-Based Approach
   start = clock();
   for (int i=0; i<200; ++i) count_loop = runLoopBased(arr, arrc, r);
   end = clock();
   time_loop = ((double)(end - start)) / CLOCKS_PER_SEC;

   // Print Results
   fprintf(stderr, "Number of Combinations (Recursive):  %d\n", count_rec);
   fprintf(stderr, "Execution Time (Recursive):  %f seconds\n", time_rec);

   fprintf(stderr, "Number of Combinations (Loop-Based): %d\n", count_loop);
   fprintf(stderr, "Execution Time (Loop-Based): %f seconds\n", time_loop);

   fprintf(stderr, "Ratio of the time taken by the Recursive version, to the time taken by the Loop based approach: %f times\n", time_rec/time_loop);

   return 0;
}
