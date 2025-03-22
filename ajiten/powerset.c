#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    const char *s;
    struct node* prev;
};
const char *arrv[] = {"a", "b", "c", '\0'};
int arrc = sizeof(arrv)/sizeof(arrv[0]) - 1;

static void *erealloc(void *p, size_t s) {
    void *q= realloc(p, s);
    if (!q && s!=0) {
        fprintf(stderr, "*** Out of memory in realloc(%ld)\n", (long)s);
        exit(20);
    }
    return q;
}

static char *estrdup(const char *p) {
    if (!p) return NULL;
    size_t l= strlen(p);
    char *q= malloc(l+1);
    if (!q) {
        fprintf(stderr, "*** Out of memory in strdup (strlen=%ld)\n", (long)l);
        exit(12);
    }
    if (l) memcpy(q, p, l);
    q[l]= '\0';
    return q;
}

typedef struct StringList {
    size_t allocated;
    size_t used;
    const char **ptr;
} StringList;

StringList *StringList_Alloc()
{
    return (StringList *)calloc(1, sizeof (StringList));
}

void StringList_Free(StringList *l)
{
    for (size_t i= 0; i<l->used; ++i) {
        free((char *)l->ptr[i]);
    }
    if (l->allocated>0) free(l->ptr);
    free(l);
}

void StringList_Add(StringList *l, const char *ptr)
{
    if (l->used==l->allocated) {
        size_t req= 32;
        if (l->allocated>0) req= 2*l->allocated;
        l->ptr= erealloc(l->ptr, req*sizeof(l->ptr[0]));
        l->allocated= req;
    }
    l->ptr[l->used]= estrdup(ptr);
    ++l->used;
}

void StringList_Print(StringList *l)
{
    for (size_t i=0; i<l->used; ++i) {
        printf("%s\n", l->ptr[i]);
    }
}

void powerset_core(const char **v, int n, StringList *into, struct node *up)
{
    struct node me = {NULL, NULL};

    if (n==0) {
        char workbuff[65536]; /* fixme: should be dynamic */
        char *p= workbuff;
        p += sprintf(p, "[");
        while (up) {
            p += sprintf(p, " %s", up->s);
            up = up->prev;
        }
        p += sprintf(p, " ]");
        StringList_Add(into, workbuff);
    } else {
        me.s = *v;
        me.prev = up;
        powerset_core(v + 1, n - 1, into, up);
        powerset_core(v + 1, n - 1, into, &me);
    }
}

StringList *powerset(const char **v, int n)
{
    StringList *into= StringList_Alloc();
    powerset_core(v, n, into, NULL);
    return into;
}

int main(int argc, char **argv)
{
    StringList set= {0, 0, NULL};
    StringList *result1= NULL;
    StringList *result2= NULL;

    /* skip progname */
    --argc;
    ++argv;

    if (argc>0) {       /* using command-line arguments */
        set.ptr= (const char **)argv;
        set.used= argc;
    } else {            /* using builtin values */
        set.used= arrc;
        set.ptr= arrv;
    }

    printf("\n== set ==\n");
    StringList_Print(&set);

    result1= powerset(set.ptr, set.used);
    printf("\n== powerset ==\n");
    StringList_Print(result1);

    result2= powerset(result1->ptr, result1->used);
    printf("\n== powerset of powerset ==\n");
    StringList_Print(result2);

    StringList_Free(result1);
    StringList_Free(result2);

    return 0;
}
