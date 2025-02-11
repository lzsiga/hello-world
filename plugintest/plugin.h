/* plugin.h */

#ifndef PLUGIN_H
#define PLUGIN_H

typedef struct PluginPublicData PluginPublicData;
typedef struct CatData CatData;
typedef struct DogData DogData;

struct PluginPublicData {
    CatData *(*Cat_Alloc) (PluginPublicData *p, const char *name);
    int      (*Cat_Free)  (PluginPublicData *p, CatData *);
    DogData *(*Dog_Alloc) (PluginPublicData *p, const char *name);
    int      (*Dog_Free)  (PluginPublicData *p, DogData *);
    int      (*Fight)     (PluginPublicData *p, DogData *, CatData *);
};

PluginPublicData *PluginInit (void);
int PluginTerm (PluginPublicData *p);

#endif
