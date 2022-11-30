#include "../../include/detection/distorsion.h"

struct corner{
    int x1,y1;
    int x2,y2;
    int x3,y3;
    int x4,y4;
};

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

double max(double value, double value1)
{
     /*
       return max value between two
      */
     if(value >value1)
	  return value;
     return value1;
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

void perspective_mat(int src[4][2], double dst[4][2], double **matr, double **matr_inv)
{
     /*
       find result of inequation between matrice to find all the new coordonate
      */
     double to_invert[][9] = {
	  { -src[0][0], -src[0][1], -1, 0, 0, 0, src[0][0] * dst[0][0],
	    src[0][1] * dst[0][0], dst[0][0] },
	  { 0, 0, 0, -src[0][0], -src[0][1], -1, src[0][0] * dst[0][1],
	    src[0][1] * dst[0][1], dst[0][1] },
	  { -src[1][0], -src[1][1], -1, 0, 0, 0, src[1][0] * dst[1][0],
	    src[1][1] * dst[1][0], dst[1][0] },
	  { 0, 0, 0, -src[1][0], -src[1][1], -1, src[1][0] * dst[1][1],
	    src[1][1] * dst[1][1], dst[1][1] },
	  { -src[2][0], -src[2][1], -1, 0, 0, 0, src[2][0] * dst[2][0],
	    src[2][1] * dst[2][0], dst[2][0] },
	  { 0, 0, 0, -src[2][0], -src[2][1], -1, src[2][0] * dst[2][1],
	    src[2][1] * dst[2][1], dst[2][1] },
	  { -src[3][0], -src[3][1], -1, 0, 0, 0, src[3][0] * dst[3][0],
	    src[3][1] * dst[3][0], dst[3][0] },
	  { 0, 0, 0, -src[3][0], -src[3][1], -1, src[3][0] * dst[3][1],
	    src[3][1] * dst[3][1], dst[3][1] },
	  { 0, 0, 0, 0, 0, 0, 0, 0, 1 }
     };

     double res[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };

     double invert[9][9] = { 0 };
     //inverse to_invert in invert
     inverse_mat(to_invert,invert);

     double *unknow = calloc(9, sizeof(double));
     //mult invert with res and unknow
     mult_mat_np(invert,res,unknow,9);
     int v = 0;
     for(int i = 0; i<3;i++)
     {
	  for(int j = 0; j<3; j++)
	  {
	       matr[i][j] = unknow[v];
	       v+=1;
	  }
     }
     inverse_mat3(matr, matr_inv);
     free(unknow);
}

struct corner find_corner(SDL_Surface *img)
{
     /*
       Find the 4 white points closest to the corners of the image
      */
     Uint32 pixel;
     Uint8 r,g,b;

     struct corner cor = {0,0,0,0,0,0,0,0};

     int max1 = img->w;
     int max2 = img->w;
     int max3 = img->w;
     int max4 = img->w;
     for(int i = 0;i<img->w;i++)
     {
	  for(int j = 0;j<img->h;j++)
	  {
	       pixel = get_pixel(img,i,j);
	       SDL_GetRGB(pixel,img->format,&r,&g,&b);
	       if(r==255)
	       {
		    if(sqrt(i*i + j*j)<max1)
		    {
			 max1 = sqrt(i*i + j*j);
			 cor.x1 = i;
			 cor.y1 = j;
		    }
		    if(sqrt((img->w-i)*(img->w-i) + j*j)<max2)
		    {
			 max2 = sqrt((img->w-i)*(img->w-i) + j*j);
			 cor.x2 = i;
			 cor.y2 = j;
		    }
		    if(sqrt((img->w-i)*(img->w-i) + (img->h-j)*(img->h-j))<max3)
		    {
			 max3 = sqrt((img->w-i)*(img->w-i) + (img->h-j)*(img->h-j));
			 cor.x3 = i;
			 cor.y3 = j;
		    }
		    if(sqrt(i*i + (img->h-j)*(img->h-j))<max4)
		    {
			 max4 = sqrt(i*i + (img->h-j)*(img->h-j));
			 cor.x4 = i;
			 cor.y4 = j;
		    }
	       }
	  }
     }
     return cor;
}

SDL_Surface *correct_perspective(SDL_Surface *img, SDL_Surface *resultat)
{
     /*
       rectifies the perspective of the image so that our grid is straight
       -img is for the research
       -resultat is the reserve of pixel to reput
      */
     Uint32 pixel;
     Uint32 newpixel;

     newpixel = SDL_MapRGB(img->format, 255, 255, 255);

     int base_coor[4][2] = {{0, 0},
			    {0, 0},
			    {0, 0},
			    {0,0}};

     struct corner cor = find_corner(img);

     base_coor[0][0] = cor.x1;
     base_coor[0][1] = cor.y1;
     base_coor[1][0] = cor.x2;
     base_coor[1][1] = cor.y2;
     base_coor[2][0] = cor.x3;
     base_coor[2][1] = cor.y3;
     base_coor[3][0] = cor.x4;
     base_coor[3][1] = cor.y4;

     double distance[4][2] = {{0,0},
			      {img->w, 0},
			      {img->w,img->h},
			      {0,img->h}};

     double **result_mat = allocMat(3);
     double **result_mat_inv = allocMat(3);

     //Perspective matrix
     perspective_mat(base_coor, distance, result_mat, result_mat_inv);

     SDL_Surface *result = SDL_CreateRGBSurface(0,img->w,img->h,32,0,0,0,0);

     for(int i = 0;i<result->w; i++)
     {
	  for(int j = 0;j<result->h; j++)
	  {
	       double wt = i;
	       double ht = j;
	       double o = 1;

	       double old[3] = {wt,ht,o};
	       double new[3] = {0,0,0};

	       //mult inverse with old and new
	       mult_mat(result_mat_inv, old, new, 3);

	       int x = (int)(new[0] / new[2]);
	       int y = (int)(new[1] / new[2]);

	       if (x >= 0 && y >= 0 && x < img->w && y < img->h)
	       {
		    pixel = get_pixel(resultat, x, y);
		    put_pixel(result, i, j, pixel);
	       }
	       else
		    put_pixel(result, i, j, newpixel);
	  }
     }
     for(int i = 0;i<3;i++)
     {
	  free(result_mat[i]);
	  free(result_mat_inv[i]);
     }
     free(result_mat);
     free(result_mat_inv);
     SDL_FreeSurface(resultat);
     return result;
}
