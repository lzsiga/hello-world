/* plugin.h */

#ifndef PLUGIN_H
#define PLUGIN_H

typedef struct PluginPublicData PluginPublicData;
typedef struct CatData CatData;
typedef struct DogData DogData;

struct PluginPublicData {
    int      version;
    int      (*PluginTerm)(PluginPublicData *p);
    CatData *(*Cat_Alloc) (PluginPublicData *p, const char *name);
    int      (*Cat_Free)  (PluginPublicData *p, CatData *);
    DogData *(*Dog_Alloc) (PluginPublicData *p, const char *name);
    int      (*Dog_Free)  (PluginPublicData *p, DogData *);
    int      (*Fight)     (PluginPublicData *p, DogData *, CatData *);
/* version=0 ends here*/
/* place for future extensions */
/* the following fields aren't available if version<1 */
};

#define PluginPublicData_LatestVersion 0
/* increment this whenever you add new fields to PluginPublicData */

typedef PluginPublicData *PluginInitFun(void);
PluginInitFun PluginInit;

#endif
