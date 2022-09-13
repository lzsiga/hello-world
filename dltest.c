/* dltest.c */

/* build:
     gcc -o dltest -W -Wall -m64 dltest.c -ldl

   options:
     -lazy |-now     RTLD_LAZY |RTLD_NOW
     -local|-global  RTLD_LOCAL|RTLD_GLOBAL
     -noload         RTLD_NOLOAD

   parameter(s):
     shared object             'dlopen' this shared object
     shared object[symbol(s)]  'dlopen' this shared object;
                               'dlsym' the symbol(s)

   usage sample:
     ./dltest -global -now \
          libm.so.6[asin,acos,atan2] \
          /usr/local/lib64/libcrypto.so.6 \
          /usr/local/lib64/libssl.so.6[TLS_method]

   output:
     Testing "libm.so.6[asin,acos,atan2]", mode=0x102
        dlopen("libm.so.6",0x102)=0x110b060
        dlsym(0x110b060,asin)=0x7fda86170a80
        dlsym(0x110b060,acos)=0x7fda86170a10
        dlsym(0x110b060,atan2)=0x7fda86170ac0

     Testing "/usr/local/lib64/libcrypto.so.3", mode=0x102
        dlopen("/usr/local/lib64/libcrypto.so.3",0x102)=0x110b6d0

     Testing "/usr/local/lib64/libssl.so.3[TLS_method]", mode=0x102
        dlopen("/usr/local/lib64/libssl.so.3",0x102)=0x110cb60
        dlsym(0x110cb60,TLS_method)=0x7fda86b8b4e0
*/

#include <errno.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static struct {
    int local;       /* 0/1: RTLD_LOCAL/RTLD_GLOBAL */
    int lazy;        /* 0/1: RTLD_NOW/RTLD_LAZY */
    int noload;      /* 0/1: 0/RTLD_NOLOAD */
    int sleep;       /* wait after the last operation (sec) */
} opt = {
    0,
    0,
    0,
    0
};

static void ParseArgs (int *pargc, char ***pargv);

static void Test1 (const char *plibname);

typedef struct Data {
    char *ptr;
    size_t len;
} Data;

int main (int argc, char **argv)
{
    int i;

    ParseArgs (&argc, &argv);

    if (argc==1) {
        fprintf (stderr, "usage: %s lib[symbol...] ... \n"
                "'NULL' as libname means current program\n"
                "\nextra:\n"
                "\tsymbol-I means: execute as function, expect 'int' return value\n"
                "\tsymbol-S means: execute as function, expect 'char *' return value\n"
                "\tsymbol-V means: execute as function, don't expect return value\n"
                , argv[0]);
        return 1;
    }

    for (i=1; i<argc; i++) {
        Test1 (argv[i]);
    }

    if (opt.sleep) {
        sleep (opt.sleep);
    }

    return 0;
}

static char *estrdup(const char *p) {
    if (!p) return NULL;
    size_t l= strlen(p);
    char *q= malloc(l+1);
    if (!q) {
        fprintf(stderr, "estrdup: *** out of memory\n");
        exit(12);
    }
    if (l) memcpy(q, p, l);
    q[l]= '\0';
    return q;
}

static void Test1 (const char *plibname)
{
    char *libname = estrdup (plibname);
    const char *dllib;
    void *handle;
    void (*fun)(void);
    BuffData elist = EmptyBuffData;
    char *symlist;
    int MY_RTLD;
    int rc;

    MY_RTLD = 0;

    if (opt.lazy) MY_RTLD |= RTLD_LAZY;
    else          MY_RTLD |= RTLD_NOW;

    if (opt.local) MY_RTLD |= RTLD_LOCAL;
    else           MY_RTLD |= RTLD_GLOBAL;

#ifdef RTLD_NOLOAD
    if (opt.noload) MY_RTLD |= RTLD_NOLOAD;
#endif

#ifdef RTLD_MEMBER
    MY_RTLD |= RTLD_MEMBER;
#endif

    fprintf (stderr, "\ntesting %s, mode=0x%x\n", plibname, MY_RTLD);

    symlist= strchr (libname, '[');
    if (symlist) {
        *symlist = '\0';
        elist.ptr= symlist+1;
        elist.len= strlen (elist.ptr);
        if (elist.len>0 && elist.ptr[elist.len-1]==']') {
            elist.ptr[--elist.len]= '\0';
        }
    }

    if (strcasecmp (libname, "NULL")==0) dllib= NULL;
    else                                 dllib= libname;

    handle = dlopen (dllib, MY_RTLD);
    if (handle==NULL) {
        int ern= errno;

        fprintf (stderr, "\tdlopen(%s) failed errno=%d (%s)\n",
                 libname, ern, dlerror ());
        return;
    }

    fprintf (stderr, "\tdlopen(%s)=%p\n", libname, handle);
    if (!handle) goto RETURN;

    pp.Text = elist;
    while ((rc= PLEX (&pp))==0) {
        ConstBuffData runopt;

        fun =  (void (*)(void)) (intptr_t) dlsym (handle, pp.Item.ptr);
        if (fun==NULL) {
            fprintf (stderr, "\tdlsym(%p,%s) failed errno=%d (%s)\n",
                handle, pp.Item.ptr, errno, dlerror ());

        } else {
            fprintf (stderr, "\tdlsym(%p,%s)=%p\n", handle, pp.Item.ptr, (void *)(intptr_t)fun);
        }
    }

RETURN:
    if (libname) free (libname);
}

static void ParseArgs (int *pargc, char ***pargv)
{
    int argc;
    char **argv;
    int parse_arg;
    char *progname;

    argc = *pargc;
    argv = *pargv;
    parse_arg = 1;
    progname = argv[0];

    while (--argc && **++argv=='-' && parse_arg) {
    switch (argv[0][1]) {
        case 'g': case 'G':
             if (strcasecmp (argv[0], "-global")==0) {
                opt.local= 0;
                break;
            } else goto UNKOPT;

        case 'l': case 'L':
             if (strcasecmp (argv[0], "-lazy")==0) {
                opt.lazy= 1;
                break;
             } else if (strcasecmp (argv[0], "-local")==0) {
                opt.local= 1;
                break;
            } else goto UNKOPT;

        case 'n': case 'N':
            if (strcasecmp (argv[0], "-noload")==0) {
                opt.noload= 1;
                break;
            } else if (strcasecmp (argv[0], "-now")==0) {
                opt.lazy= 0;
                break;
            } else goto UNKOPT;

        case 's': case 'S':
            if (strcasecmp (argv[0], "-sleep")==0) {
                if (argc<2) goto MISVAL;
                --argc;
                ++argv;
                opt.sleep= atoi (argv[0]);
                break;
            } else goto UNKOPT;

        case 0:parse_arg = 0; break;
        case '-':
            if (argv[0][2]=='\0') {
                parse_arg = 0;
                break;
            }
            goto UNKOPT;

        default: UNKOPT:
            fprintf (stderr, "Unknown option '%s'\n", *argv);
            exit (4);

        MISVAL:
            fprintf (stderr, "Option '%s' without value\n", *argv);
            exit (4);
        }
    }
    ++argc;
    *--argv = progname;
    *pargc = argc;
    *pargv = argv;
}
