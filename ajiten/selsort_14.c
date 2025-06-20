/* selsort_14.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArray(int arr[], int size){
    int i;
    for (i = 0; i < size; i++)
        printf(" %d", arr[i]);
    printf ("\n");
}

void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n) {
    // Start with the whole array as unsorted one, and sequentially move the unsorted subarray towards right
    for (int i = 0; i < n - 1; i++) {
        printf("====Pass no: %d, i: %d, j: %d..%d ====================\n", i+1, i, i+1, n-1);
        printArray(arr, n);

        //Find the minimum element in unsorted array
        int min_pos= i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_pos]) {
                printf("<<>>minimum found: arr[%d]=%d less than arr[%d]=%d; no swap yet\n",
                       j, arr[j], min_pos, arr[min_pos]);
                min_pos= j;
            }
        }
        printf("<<>>swap arr[%d]=%d and arr[%d]=%d\n",
               i, arr[i], min_pos, arr[min_pos]);
        swap(&arr[i], &arr[min_pos]);
        printArray(arr, n);
     }
}

static void makeSpace(int **parr, int *pn, int *pnAlloc)
{
    if      (*pn > *pnAlloc) exit(12);
    else if (*pn < *pnAlloc) return;
    if (*pnAlloc==0) *pnAlloc= 8;
    else             *pnAlloc= *pnAlloc * 2;
    *parr= realloc (*parr, *pnAlloc * sizeof(int));
    if (*parr==NULL) exit(13);
}

static void arrayFromArgs(int argc, char **argv, int **parr, int *pn)
{
    int nAlloc= 0, n= 0, *arr= NULL;

    for (int i=1; i<argc; ++i) {
        char *endptr= NULL;
        int val= (int)strtol(argv[i], &endptr, 10);
        if (*endptr == '\0') { /* valid number */
            makeSpace(&arr, &n, &nAlloc);
            arr[n++]= val;
        }
    }
    *parr= arr;
    *pn= n;
}

static void arrayFromStdin(int **parr, int *pn)
{
    int nAlloc= 0, n= 0, *arr= NULL;

    printf ("Enter numbers, EOF to end\n");
    for (int done= 0; !done; ) {
        int val;
        int nscan= scanf("%d", &val);
        if (nscan==1) {
            makeSpace(&arr, &n, &nAlloc);
            arr[n++]= val;
        } else {
            done= 1;
        }
    }
    *parr= arr;
    *pn= n;
}

int main(int argc, char **argv) {
    int defArr[8] = { 5, 1, 3, 16, 8, 12, 11, 2 };
    int *arr;
    int n;

    if (argc>1) {
        arrayFromArgs(argc, argv, &arr, &n);
    } else {
        arrayFromStdin(&arr, &n);
    }

    if (n==0) {
        printf("Okay, using buitin default\n");
        arr= malloc (sizeof(defArr));
        if (!arr) exit(14);
        memcpy (arr, defArr, sizeof(defArr));
        n= sizeof(defArr)/sizeof(defArr[0]);
    }

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

    if (arr) free(arr);
    return 0;
}
