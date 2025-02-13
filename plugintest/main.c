/* main.c */

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "plugin.h"

int main (void) {
    const char *pluginfname= "./libplugin.so";
    const char *initname= "PluginInit";
    const int reqver= 0; /* we don't use functions from newer versions */
    void *h;
    PluginInitFun *initfun= NULL;
    PluginPublicData *p= NULL;
    CatData *c1, *c2, *c3;
    DogData *d1, *d2, *d3;

    srandom (time (NULL));

    h= dlopen (pluginfname, RTLD_LAZY);
    if (!h) {
        fprintf (stderr, "%s:%s:%d dlopen(\"%s\") failed: %s\n",
                 __FILE__, __func__, __LINE__,
                 pluginfname, dlerror());
        return 0;
    }

    initfun= (PluginInitFun *)(intptr_t)dlsym (h, initname);
    if (!initfun) {
        fprintf (stderr, "%s:%s:%d dlsym(\"%s\", \"%s\") failed: %s\n",
                 __FILE__, __func__, __LINE__,
                 pluginfname, initname, dlerror());
        goto DLCLOSE;
    }
    p= (*initfun)();
    if (!p) goto DLCLOSE;

    fprintf (stderr, "%s:%s:%d %s has been loaded, %s successfully executed, version=%d\n",
             __FILE__, __func__, __LINE__,
             pluginfname, initname, (int)p->version);
    if (p->version<reqver) {
        fprintf (stderr, "%s:%s:%d Upgrade your plugin to version %d\n",
                 __FILE__, __func__, __LINE__,
                 reqver);
        goto DLCLOSE;
    }

    c1= p->Cat_Alloc (p, "Bella");
    c2= p->Cat_Alloc (p, "Chloe");
    c3= p->Cat_Alloc (p, "Sasha");

    d1= p->Dog_Alloc (p, "Cash");
    d2= p->Dog_Alloc (p, "Frisco");
    d3= p->Dog_Alloc (p, "Gator");

    p->Fight (p, d1, c3);
    p->Fight (p, d2, c2);
    p->Fight (p, d3, c1);

    p->Cat_Free (p, c1);
    p->Cat_Free (p, c2);
    p->Cat_Free (p, c3);

    p->Dog_Free (p, d1);
    p->Dog_Free (p, d2);
    p->Dog_Free (p, d3);

    p->PluginTerm (p);

DLCLOSE:;
    dlclose (h);
    return 0;
}
