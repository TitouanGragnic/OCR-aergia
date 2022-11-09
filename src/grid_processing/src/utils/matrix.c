#include "../../include/utils/matrix.h"

double **allocMat(int size)
{
     /*
       return an allocation for a matrice(sizexsize)
     */
    double **mat = calloc(size, sizeof(double *));
    for (int i = 0; i < size; i++)
    {
        mat[i] = calloc(size, sizeof(double));
    }
    return mat;
}

void mult_mat(double **mat, double *v, double *tab_out, int size)
{
     /*
       multiply all the value in mat and v
       put the sum of this in v_out
      */
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            tab_out[i] += mat[i][j] * v[j];
        }
    }
}


void cofactor(double matrice[9][9], double temp[9][9], int p, int q, int n)
{
     /*
       find cofactor for adjoint matrice
      */
     int i = 0;
     int j = 0;
     for(int row=0;row<n;row++)
     {
	  for(int col=0;col<n;col++)
	  {
	       if(row != p && col != q)
	       {
		    temp[i][j++] = matrice[row][col];

		    if(j == n-1)
		    {
			 j=0;
			 i++;
		    }
	       }
	  }
     }
}

double determinant(double matrice[9][9], int n)
{
     /*
       find determinant to determine inverse matrice
      */
     double d = 0;

     if(n==1)
	  return matrice[0][0];
     double temp[9][9];
     int sign = 1;

     for(int f = 0;f<n;f++)
     {
	  cofactor(matrice,temp,0,f,n);
	  d+=sign*matrice[0][f]*determinant(temp, n-1);

	  sign = -sign;
     }

     return d;
}

void adjoint(double matrice[9][9], double adj[9][9])
{
     /*
       return matrice adjoint to matrice
     */
     int sign = -1;
     double temp[9][9];

     for(int i = 0;i<9;i++)
     {
	  for(int j = 0;j<9;j++)
	  {
	       cofactor(matrice, temp,  i, j, 9);
	       sign = ((i+j)%2 == 0) ? 1 : -1;
	       adj[j][i] = (sign)*(determinant(temp,9-1));
	  }
     }
}

void inverse(double matrice[9][9], double inverse[9][9])
{
     /*
       fill inverse matrice from matrice
      */
     int det = determinant(matrice,9);
     double adj[9][9];
     adjoint(matrice, adj);

     for(int i = 0; i<9;i++)
     {
	  for(int j=0;j<9;j++)
	  {
	       inverse[i][j] = adj[i][j]/det;
	  }
     }
}

void inverse_mat(double matrice[9][9], double inv[9][9])
{
     /*
       launch process to inverse matrice
      */
    double adj[9][9];
    adjoint(matrice, adj);
    inverse(matrice, inv);
}

void mult_mat_np(double mat[][9], double v[9], double tab_out[9], int size)
{
     /*
       multiply all the value in mat and v
       put the sum of this in v_out
      */
     for (int i = 0; i < size; i++)
     {
	  for (int j = 0; j < size; j++)
	  {
	       tab_out[i] += mat[i][j] * v[j];
	  }
     }
}

void inverse_mat3(double **mat, double **m_inv)
{
     /*
       inverse 3x3 matrice with precalculate formule
      */
     double divisor =
	  mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0]
	  + mat[0][2] * mat[2][1] * mat[1][0] - mat[0][2] * mat[1][1] * mat[2][0]
	  - mat[0][1] * mat[1][0] * mat[2][2] - mat[0][0] * mat[2][1] * mat[1][2];

     double new_mat[3][3] = {
	  { mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1],
	    mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2],
	    mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1] },
	  { mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2],
	    mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0],
	    mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2] },
	  { mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0],
	    mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1],
	    mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0] } };

     for (int i = 0; i < 3; i++)
	  for (int j = 0; j < 3; j++)
	       m_inv[i][j] = new_mat[i][j] / divisor;
}
