/* main.c */

/*
   Expected behavior (14.0.50.20221220-git):

   gcc -g -o main main.c foo.c bar.c
   gdb ./main
   (gdb) print duplicate
   $1 = 0x202c "foo"
   (gdb) print 'foo.c'::duplicate
   $2 = 0x202c "foo"
   (gdb) print 'bar.c'::duplicate
   $3 = 0x2030 "bar"
*/

#include <stdio.h>

extern const char *fooFun(void);
extern const char *barFun(void);

int main (void) {
    printf ("fooFun returned %s\n", fooFun());
    printf ("barFun returned %s\n", barFun());
    return 0;
}
