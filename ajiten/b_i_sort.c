
// C program for implementation of 
// binary insertion sort
#include <stdio.h>
#include <string.h>
 
// A binary search based function
// to find the position
// where item should be inserted 
// in a[low..high]
int binarySearch(int a[], int item, 
                 int low, int high)
{
    if (high <= low)
        return (item > a[low]) ? 
                (low + 1) : low;
 
    int mid = (low + high) / 2;
 
    if (item == a[mid])
        return mid + 1;
 
    if (item > a[mid])
        return binarySearch(a, item, 
                            mid + 1, high);
    return binarySearch(a, item, low, 
                        mid - 1);
}
 
// Function to sort an array a[] of size 'n'
void insertionSort(int a[], int n)
{
    int i, loc, j, selected;
 
    for (i = 1; i < n; ++i) 
    {
        j = i - 1;
        selected = a[i];
 
        // find location where selected should be inserted
        loc = binarySearch(a, selected, 0, j);
 
        // Move all elements after location to create space
        // a[loc..j] ==> a[loc+1..j+1]

#ifdef LOOPCOPY
        for (; j >= loc; --j)
        {
            a[j+1] = a[j];
        }
#else
        if (j+1>loc) {
            memmove (&a[loc+1], &a[loc], (j+1-loc)*sizeof a[0]);
        }
#endif
        a[loc] = selected;
    }
}
 
// Driver Code
int main(void)
{
    int a[]
        = { 37, 23, 0, 17, 12, 72, 31, 46, 100, 88, 54 };
    int n = sizeof(a) / sizeof(a[0]), i;
 
    insertionSort(a, n);
 
    printf("Sorted array: \n");
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
    fputc('\n', stdout);
    return 0;
}
