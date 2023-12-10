/* arpi_esp_mat.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 0
#define Y 1
#define Z 2
#define W 3

typedef float c_MATRIX[3][4];

static void phex(const c_MATRIX m) {
    int i,j;
    for(i=0;i<3;i++) {
        for (j=0; j<4; ++j) {
            printf(" %6g", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

#define ast3d_SWAP_YZ 1

static void mat_swap (c_MATRIX a)
{
/*
  swap_mat: swap y/z in matrix.
*/
  int   i;
  float tmp;
#ifdef ast3d_SWAP_YZ
  phex(a);
  for (i = 0; i < 3; i++) { /* swap columns */
    tmp = a[i][Y];
    a[i][Y] = a[i][Z];
    a[i][Z] = tmp;
  }
  phex(a);
  for (i = 0; i < 4; i++) { /* swap rows */
    tmp = a[Y][i];
    a[Y][i] = a[Z][i];
    a[Z][i] = tmp;
  }
  phex(a);
#endif
}

static void mat_identity (c_MATRIX mat) {
  memset (mat, 0, sizeof(c_MATRIX));
  mat[0][0]= mat[1][1]= mat[2][2]= 1;
}

int main (void) {
  int       i, j;
  c_MATRIX  mat;

  mat_identity (mat);
  mat[0][0]= 100; mat[0][1]= 101; mat[0][2]= 102; mat[0][3]= 103;
  mat[1][0]= 110; mat[1][1]= 111; mat[1][2]= 112; mat[1][3]= 113;
  mat[2][0]= 120; mat[2][1]= 121; mat[2][2]= 122; mat[2][3]= 123;
/*for (i = 0; i < 3; i++)
  for (j = 0; j < 3; j++)
      if (afs_fread_float(&(mat[i][j]), sizeof (float), 1, f) != 1) return ast3d_err_badfile;
 */

//  mat_swap (mat);
#ifdef ast3d_SWAP_YZ
  phex(mat);
  for (i = 0; i < 3; i++) { /* swap columns */
    float tmp = mat[i][Y];
    mat[i][Y] = mat[i][Z];
    mat[i][Z] = tmp;
  }
  phex(mat);
  for (i = 0; i < 4; i++) { /* swap rows */
    float tmp = mat[Y][i];
    mat[Y][i] = mat[Z][i];
    mat[Z][i] = tmp;
  }
  phex(mat);
#endif
}
