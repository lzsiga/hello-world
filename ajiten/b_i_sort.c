// C program for implementation of binary insertion sort
#include <stdio.h>
// A binary search based function to find the position where item should be inserted in a[low..high]
int binarySearch(int a[], int item, int low, int high)
{
    printf("binary-search: item: %d, low: %d, a[%d]: %d, high:%d\n", item, low, low, a[low], high);
    if (high <= low) 
    {
        printf("high <= low\n"); 
        return (item > a[low]) ? (low + 1) : low;     
    }
    int mid = (low + high) / 2;  
    printf("2. mid: %d, a[%d]: %d\n", mid, mid, a[mid]);
    if (item == a[mid]) 
    {
        printf("item == a[mid]:: mid+1: %d\n", mid+1);   
        return mid + 1;
    }
    if (item > a[mid])  
    {
          printf("item > a[mid]:: mid+1: %d\n", mid+1); 
          return binarySearch(a, item, mid + 1, high);
    }
    printf("------New nested/recursive call occurs------\n");
    return binarySearch(a, item, low, mid - 1);
}

// Function to sort an array a[] of size 'n'
void insertionSort(int a[], int n)
{
    int i, loc, j, k, selected;
    for (i = 7; i <= 7; ++i)      //for (i = 1; i < n; ++i) 
    {
        j = i - 1;         selected = a[i];               
        printf("selected: a[%d]: %d\n", i, a[i]);
        // find location where selected should be inserted, 
        printf("Before calling  BINSEARCH: <loc of start of subarray:> %d, a[%d]: %d, selected: %d, j: %d\n", loc, i, a[i], selected, j);
        loc = binarySearch(a, selected, 0, j);            
        printf("After calling  BINSEARCH: <loc returned:> %d, a[%d]: %d, selected: %d, j: %d\n", loc, i, a[i], selected, j);//, a[loc]); 
        // Move all elements after location to create space
        while (j >= loc) 
        {                          
            printf("BEFORE> j: %d, a[%d]: %d, a[%d]: %d\n", j, j+1, a[j+1], j, a[j]);
            a[j + 1] = a[j];       
            printf("AFTER> j: %d, a[%d]: %d, a[%d]: %d\n", j, j+1, a[j+1], j, a[j]);
            j--;
        }
        a[j + 1] = selected;      
        printf("j: %d, a[%d]: %d, selected: %d\n", j, j+1, a[j+1], selected);
    }
}
 
// Driver Code
int main()
{
    int a[] = {1,3,3,3,4,4,5,2,11,0,50,47};      //{ 37, 23, 0, 17, 12, 72, 31, 46, 100, 88, 54 };
    int n = sizeof(a)/sizeof(a[0]), i; 
    printf("size of array: %d\n", n);
    int j = 0; 
    while(j<n) {
        printf(" -a[%d]: %d", j,a[j]);  
        j++;
    } 
    printf("\n");
    insertionSort(a, n);
    printf("Sorted array: \n");
    for (i = 0; i < n; i++)
        printf("%d ", a[i]);
 
    return 0;
}
