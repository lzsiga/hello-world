/* test01.c */

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char **argv) {
    char *progname= argv[0];
    int rc, ern;

    if (argc<2) {
        fprintf(stderr, "usage: %s <username>\n", progname);
        exit(0);
    }
    const char *username= argv[1];

    struct passwd *pwd = getpwnam(username);
    if (pwd == NULL) {
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "getpwname(%s) returned %p\n",
        username, (void *)pwd);

    errno= 0;
    rc= seteuid(pwd->pw_uid);
    ern= errno;
    fprintf(stderr, "seteuid(%lld) returned %d errno=%d %s\n",
        (long long)pwd->pw_uid, rc, ern, strerror(ern));
    if (rc) {
        exit(EXIT_FAILURE);
    }

    errno= 0;
    rc= setreuid(-1, pwd->pw_uid);
    ern= errno;
    fprintf(stderr, "setreuid(-1, %lld) returned %d errno=%d %s\n",
        (long long)pwd->pw_uid, rc, ern, strerror(ern));
    if (rc) {
        exit(EXIT_FAILURE);
    }

    return 0;
}
