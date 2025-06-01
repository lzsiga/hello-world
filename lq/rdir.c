#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <dirent.h>

#define UNIX

union dir_st {
#ifdef UNIX
    struct dirent *dir;
#else
    struct _finddata_t dir;
#endif
};

union dirptr {
#ifdef UNIX
    DIR *curdir;
#else
    intptr_t curdir;
#endif
};


/*  static inline union dirptr flp__open_directory(const char *target, union dir_st *dir_st)
 *
 *  Open a directory syscall wrapper.
 *
 *  RETURN: union dirptr
 */

static inline union dirptr flp__open_directory(const char *target, union dir_st *dir_st) {
    union dirptr dirptr;

#ifdef UNIX
    /* Unix syscall */
    dirptr.curdir = opendir(target);

    printf("%s(): dirptr.curdir = %p target = %s\n",__func__,dirptr.curdir,target);

#else
    char temp[PATH_MAX] = {0};

    strcpy(temp, target);
    strcat(temp, "\\*");
    /* Windows syscall */
    dirptr.curdir = _findfirst(temp, &dir_st->dir);
#endif

    return dirptr;
}

/*  static inline union dir_st flp__read_directory(union dirptr *dirptr, union dir_st *dir_st)
 *
 *  Enumerate a directory syscall wrapper.
 *
 *  RETURN: union dir_st
 */

static inline union dir_st flp__read_directory(union dirptr *dirptr, union dir_st *dir_st) {
#ifdef UNIX
    puts("just before syscall\n");
    printf("%s(): dirptr.curdir = %p\n",__func__,dirptr->curdir);
    //exit(0);
    perror("Error:");

    /* Unix syscall */
    dir_st->dir = readdir(dirptr->curdir); // segfault
#else
    char temp[PATH_MAX] = {0};

    strcpy(temp, target);
    strcat(temp, "\\*");
    /* Windows syscall */
    dir_st->dir = _findnext(dirptr->curdir, &dir_st->dir);
#endif

    puts("just before return\n");

    return *dir_st;
}

/*  static inline void flp__close_directory(union dirptr dirptr)
 *
 *  Close a directory syscall wrapper.
 *
 *  RETURN: int
 */

static inline int flp__close_directory(union dirptr dirptr) {
    short int retval = -1;

#ifdef UNIX
    /* Unix syscall */
    retval = closedir(dirptr.curdir);
#else
    /* Windows syscall */
    retval = _findclose(dirptr.curdir);
#endif

    return retval;
}

int main(void) {
    union dirptr dirptr;
    union dir_st dir_st;

    dirptr = flp__open_directory("[removed]", NULL);

    printf("%s(): just before read call\n",__func__);

    //dir_st.dir = readdir(dirptr.curdir); // works, no segfault

    flp__read_directory(&dirptr, NULL); // segfault

    return 0;
}
