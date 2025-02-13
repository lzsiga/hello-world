/* plugin.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin.h"

typedef struct PluginData PluginData;

typedef struct DataCore {
    PluginData *instance;
} DataCore;

struct CatData {
    PluginData *instance;
    size_t id;
    char *name;
};

struct DogData {
    PluginData *instance;
    size_t id;
    char *name;
};

struct PluginData {
    PluginPublicData pub;
    size_t dogcnt; /* number of active dogs */
    size_t catcnt; /* number of active cats */
    size_t idsrc;  /* for generating 'id'-s */
};

static int      PluginTerm(PluginPublicData *pp);
static CatData *Cat_Alloc (PluginPublicData *pp, const char *name);
static int      Cat_Free  (PluginPublicData *pp, CatData *);
static DogData *Dog_Alloc (PluginPublicData *pp, const char *name);
static int      Dog_Free  (PluginPublicData *pp, DogData *);
static int      Fight     (PluginPublicData *pp, DogData *, CatData *);

static void *emalloc (size_t s)
{
    void *p= malloc(s);
    if (s!=0 && p==NULL) {
        fprintf(stderr, "%s:%s:%d *** Out of memory at malloc(%ld)\n",
                __FILE__, __func__, __LINE__, (long)s);
        exit(1);
    }
    return p;
}

static char *estrdup (const char *p)
{
    char *q= NULL;
    size_t len= 0;

    if (!p) return NULL;
    len= strlen (p);
    q= emalloc (len+1);
    if (!q) {
        fprintf(stderr, "%s:%s:%d *** Out of memory at strdup(\"%s\")\n",
                __FILE__, __func__, __LINE__, p);
        exit(1);
    }
    if (len>0) memcpy (q, p, len);
    q[len]= '\0';
    return q;
}

static void *ecalloc (size_t n, size_t s)
{
    void *p= calloc(n, s);
    if (n!=0 && s!=0 && p==NULL) {
        fprintf(stderr, "%s:%s:%d *** Out of memory at calloc(%ld, %ld)\n",
                __FILE__, __func__, __LINE__, (long)n, (long)s);
        exit(1);
    }
    return p;
}

PluginPublicData *PluginInit(void)
{
    PluginData *p;
    PluginPublicData *pp;
    p= ecalloc (1, sizeof *p);
    pp= &p->pub;

    pp->version=   0; /* last field of this version is 'Fight' */
    pp->PluginTerm=PluginTerm;
    pp->Cat_Alloc= Cat_Alloc;
    pp->Cat_Free=  Cat_Free;
    pp->Dog_Alloc= Dog_Alloc;
    pp->Dog_Free=  Dog_Free;
    pp->Fight=     Fight;

    return pp;
}

static int PluginTerm (PluginPublicData *pp)
{
    PluginData *p= (PluginData *)pp;
    if (!p) return 0;
    if (p->dogcnt || p->catcnt) {
        fprintf(stderr, "%s:%s:%d *** Leaving orphans: %ld dogs, %ld cats\n",
                __FILE__, __func__, __LINE__, (long)p->dogcnt, (long)p->catcnt);
    }
    memset(p, 0, sizeof *p);
    free(p);
    return 0;
}

static int InstaceCheck (const PluginData *p, const DataCore *d)
{
    int rc= 0;

    if (p != d->instance) {
        fprintf (stderr, "%s:%s:%d object %p doesn't belong to instance %p\n",
                 __FILE__, __func__, __LINE__,
                 (void *)d, (void *)p);
        rc= -1;
    }
    return rc;
}

static CatData *Cat_Alloc (PluginPublicData *pp, const char *name)
{
    CatData *cd= NULL;
    PluginData *p= (PluginData *)pp;

    if (!p) return NULL;
    cd= ecalloc (1, sizeof *cd);
    cd->instance= p;
    cd->id= ++p->idsrc;
    cd->name= estrdup (name);
    ++p->catcnt;
    return cd;
}

static int Cat_Free (PluginPublicData *pp, CatData *cd)
{
    PluginData *p= (PluginData *)pp;

    if (!p || !cd || InstaceCheck(p, (DataCore *)cd)!=0) return -1;
    free (cd->name);
    cd->name= NULL;
    memset (cd, 0, sizeof *cd);
    free (cd);
    --p->catcnt;
    return 0;
}

static DogData *Dog_Alloc (PluginPublicData *pp, const char *name)
{
    DogData *dd= NULL;
    PluginData *p= (PluginData *)pp;

    if (!pp) return NULL;
    dd= ecalloc (1, sizeof *dd);
    dd->instance= (PluginData *)p;
    dd->id= ++p->idsrc;
    dd->name= estrdup (name);
    ++p->dogcnt;
    return dd;
}

static int Dog_Free (PluginPublicData *pp, DogData *dd)
{
    PluginData *p= (PluginData *)pp;

    if (!p || !dd || InstaceCheck(p, (DataCore *)dd)!=0) return -1;
    free (dd->name);
    dd->name= NULL;
    memset (dd, 0, sizeof *dd);
    free (dd);
    --p->dogcnt;
    return 0;
}

static int Fight (PluginPublicData *pp, DogData *dd, CatData *cd)
{
    PluginData *p= (PluginData *)pp;
    int r;

    if (!p || !dd || 
        InstaceCheck(p, (DataCore *)dd)!=0 ||
        InstaceCheck(p, (DataCore *)cd)!=0) return -1;
    r= rand()%3;
    if (r==0) {
        fprintf (stderr, "cat %s (#%ld) wins again dog %s (#%ld)\n",
            cd->name, (long)cd->id, dd->name, (long)dd->id);
    } else if (r==1) {
        fprintf (stderr, "cat %s (#%ld) loses again dog %s (#%ld)\n",
            cd->name, (long)cd->id, dd->name, (long)dd->id);
    } else {
        fprintf (stderr, "cat %s (#%ld) and dog %s (#%ld) refuse to fight\n",
            cd->name, (long)cd->id, dd->name, (long)dd->id);
    }
    return 0;
}
