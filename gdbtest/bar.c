/* bar.c */

static struct {
    int barfld;
} dup_init = {
    'B'
};

static struct {
    int barfld;
} dup_zeroinit;

void *barFun() { return (void *)&dup_zeroinit; }
