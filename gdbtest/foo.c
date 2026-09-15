/* foo.c */

static struct {
    int foofld;
} dup_init = {
    'F'
};

static struct {
    int foofld;
} dup_zeroinit;

void *fooFun() { return (void *)&dup_zeroinit; }
