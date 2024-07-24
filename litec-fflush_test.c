/*
    fflush proba (3D Gamestudio Lite-C)
*/

#include <strio.c>

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>

function main() {
    var fhandle_n = 0;

    level_load(NULL);
    wait(2);

    fhandle_n = file_open_write ("bubu.txt");
diag_var("\nfhandle_n: %6.3f",fhandle_n);

    file_str_write(fhandle_n, "bubu\n\r");
diag("\njon");
diag_var("\nfhandle_n: %6.3f",fhandle_n);
    fflush(fhandle_n);        // itt akad el...
diag("\nmegy");

    file_close(fhandle_n);
}
