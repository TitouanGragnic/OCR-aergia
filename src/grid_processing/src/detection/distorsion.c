#include "../../include/detection/distorsion.h"

struct corner{
    int x1,y1;
    int x2,y2;
    int x3,y3;
    int x4,y4;
};


double max(double value, double value1)
{
     /*
       return max value between two
      */
     if(value >value1)
	  return value;
     return value1;
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
