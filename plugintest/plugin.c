/* plugin.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin.h"

typedef struct PluginData PluginData;

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
    size_t catcnt; /* number of active dogs */
    size_t idsrc;  /* for generating 'id'-s */
};

static CatData *Gate_Cat_Alloc (PluginPublicData *pp, const char *name);
static int      Gate_Cat_Free  (PluginPublicData *pp, CatData *);
static DogData *Gate_Dog_Alloc (PluginPublicData *pp, const char *name);
static int      Gate_Dog_Free  (PluginPublicData *pp, DogData *);
static int      Gate_Fight     (PluginPublicData *pp, DogData *, CatData *);

static void *emalloc(size_t s)
{
    void *p= malloc(s);
    if (s!=0 && p==NULL) {
        fprintf(stderr, "%s:%s:%d *** Out of memory at malloc(%ld)\n",
                __FILE__, __func__, __LINE__, (long)s);
        exit(1);
    }
    return p;
}

static void *ecalloc(size_t n, size_t s)
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
    p= ecalloc (1, sizeof sizeof *p);
    pp= &p->pub;

    pp->Cat_Alloc= Gate_Cat_Alloc;
    pp->Cat_Free=  Gate_Cat_Free;
    pp->Dog_Alloc= Gate_Dog_Alloc;
    pp->Dog_Free=  Gate_Dog_Free;
    pp->Fight=     Gate_Fight;

    return pp;
}

int PluginTerm (PluginPublicData *pp)
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

static CatData *Gate_Cat_Alloc (PluginPublicData *pp, const char *name)
{
    (void)pp;
    (void)name;
    return NULL;
}

static int Gate_Cat_Free (PluginPublicData *pp, CatData *cd)
{
    (void)pp;
    (void)cd;
    return 0;
}

static DogData *Gate_Dog_Alloc (PluginPublicData *pp, const char *name)
{
    (void)pp;
    (void)name;
    return NULL;
}

static int Gate_Dog_Free (PluginPublicData *pp, DogData *dd)
{
    (void)pp;
    (void)dd;
    return 0;
}

static int Gate_Fight (PluginPublicData *pp, DogData *dd, CatData *cd)
{
    (void)pp;
    (void)dd;
    (void)cd;
    return 0;
}
