#ifndef MATRIX_H_
#define MATRIX_H_

#include <err.h>
#include <stdlib.h>

double **allocMat(int size);
void mult_mat(double **mat, double *v, double *tab_out, int size);
void inverse_mat(double matrice[9][9], double inv[9][9]);
void mult_mat_np(double mat[][9], double v[9], double tab_out[9], int size);
void inverse_mat3(double **mat, double **m_inv);

#endif
