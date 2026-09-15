/* main.c */

#include <stdio.h>

extern void *fooFun (void);
extern void *barFun (void);

static struct {
    int mainfld;
} dup_zeroinit;

static struct {
    int mainfld;
} main_zeroinit;

static struct {
    int mainfld;
} main_oneinit = {
    1
};

int main (void) {
    printf ("fooFun returned %p\n", fooFun());
    printf ("barFun returned %p\n", barFun());
    return 0;
}
