/* powerset.c */

#include <limits.h>
#include <stdio.h>

#ifndef LONG_BIT
#define LONG_BIT (CHAR_BIT * sizeof(long))
#endif

/* sample usage: ./powerset apple pear cherry
   output (2^3==8 lines):
     []
     [apple]
     ...
     [apple,pear,cherry]

   sample recursive usage: ./powerset $(./powerset apple pear cherry)
   output (2^8==256 lines):
     []
     [[]]
     ...
     [[],[apple],[pear],[apple,pear],[cherry],[apple,cherry],[pear,cherry],[apple,pear,cherry]]
 */

static void PrintSubSet (unsigned long cnt, int nelems, const char **pelems) {
    int i;
    unsigned long mask= 1;
    int nprinted= 0;
    FILE *f= stdout;

    fputc ('[', f);
    for (i=0; i<=nelems; ++i, mask<<=1) {
        if ((cnt&mask)!=0) {
            if (nprinted>0) fputc(',', f);
            printf ("%s", pelems[i]);
            ++nprinted;
        }
    }
    fputc (']', f);
    fputc ('\n', f);
}

int main (int argc, char **argv) {
    int nmax= LONG_BIT;
    int nelems= argc-1;
    const char **pelems= (const char **)argv+1;
    unsigned long cnt, cntmax;

    if (nelems>nmax) {
        fprintf (stderr, "Too many elements (%d), truncating to %d\n",
            nelems, nmax);
        nelems= nmax;
    }

    if (nelems==0) cntmax= 0;
    else           cntmax= ((unsigned long)-1L)>>(LONG_BIT-nelems);
    for (cnt= 0; cnt<=cntmax; ++cnt) {
        PrintSubSet (cnt, nelems, pelems);
    }
    return 0;
}
