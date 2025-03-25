#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Function to print a combination
static void printCombination(int subset[], int r) {
    for (int i = 0; i < r; i++) {
        printf("%d ", subset[i]);
    }
    printf("\n");
}

// Recursive function to generate combinations
// arr     set elements
// n       number of set elements
// r       number of subset elements
// index   process set elements starting from arr[index]
// k       already generated subset elements (k<=r)
// subset  buffer for subset elements
static void generateCombinations_core(const int arr[], int n, int r, int index, int k, int *subset) {
    // Base case: If we have selected 'r' elements, print the combination
    if (k == r) {
        printCombination(subset, r);
        return;
    }

    // Iterate through the remaining elements in the set
    for (int i = index; i < n; i++) {
        // Include the current element in the combination
        subset[k] = arr[i];
        // Recursively call the function to generate combinations with the next element
        generateCombinations_core(arr, n, r, i + 1, k + 1, subset);
    }
}

static void generateCombinations(int arr[], int n, int r) {
    int *subset= malloc (n*(sizeof subset[0]));
    generateCombinations_core(arr, n, r, 0, 0, subset);
    free (subset);
}

// Main function
int main() {
    int r =3;
    int arrv[] = {1, 2, 3, 4, 12, 9, 5};
    int n = sizeof(arrv)/sizeof(arrv[0]);

    printf("size:%d\n", n);

    generateCombinations(arrv, n, r);

    return 0;
}
