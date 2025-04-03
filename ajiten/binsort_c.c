/* binsort_c.c */
// C program for implementation of binary insertion sort

#include <stdio.h>

// A binary search based function to find the position where item should be inserted in a[low..high]
int binarySearch(char a[], char item, int low, int high)
{
    if (high <= low)
    {
        return (item > a[low]) ? (low + 1) : low;
    }
    int mid = (low + high) / 2;
    if (item == a[mid])
    {
        return mid + 1;
    }
    if (item > a[mid])
    {
          return binarySearch(a, item, mid + 1, high);
    }
    return binarySearch(a, item, low, mid - 1);
}

// Function to sort an array a[] of size 'n'
void insertionSort(char a[], int n)
{
    int i, loc, j, k;
    char selected;
    for (i = 1; i < n; ++i)
    {
        j = i - 1;         selected = a[i];
        // find location where selected should be inserted,
        loc = binarySearch(a, selected, 0, j);
        // Move all elements after location to create space
        while (j >= loc)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}

// Driver Code
int main()
{
    char *a[] = {"S", "O","R","T", "E", "X","A","M", "P","L","E",0};
    int n = sizeof(a)/sizeof(a[0])-1, i;
    printf("size of array: %d\n", n);
    int j = 0;
    while(j<n) {
        printf(" -a[%d]: %s", j,a[j]);
        j++;
    }
    printf("\n");
    insertionSort(a, n);
    printf("Sorted array: \n");
    for (i = 0; i < n; i++)
        printf("a[%d]:%s ", i,a[i]);

    return 0;
}
