/*
    fflush proba (3D Gamestudio Lite-C)
*/

#include <strio.c>

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>

function main() {
    FILE *fhandle_n = NULL;

    level_load(NULL);
    wait(2);

    fhandle_n = fopen("bubu.txt", "at");
diag_var("\nfhandle_n: %6.3f",fhandle_n);

    fprintf(fhandle_n, "bubu\n");
diag("\njon");
diag_var("\nfhandle_n: %6.3f",fhandle_n);
    fflush(fhandle_n);        // itt akad el...
diag("\nmegy");

    fclose(fhandle_n);
}
