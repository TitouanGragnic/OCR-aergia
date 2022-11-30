#include "../../include/color_treatment/edge_detection.h"

int get_new_value(SDL_Surface *img,int i, int j)
{
     /*
       return if point(i,j) is black or white
      */
    int w = img->w;
    int h = img -> h;
     if(pixelValue(i, j, img) != 0)
	  return 1;
     for(int di = -1;di<2;di+=2)
	  for(int dj = -1;dj<2;dj+=2)
	       if ( i+di>0 && i+di<w && j+dj>0 && j+dj<h &&   pixelValue(i+di, j+dj, img) != 0)
		    return 0;
     return 1;
}

double convolution(SDL_Surface *img , double matr[3*3], int x, int y)
{
     /*
       return result on convolution of matr in point(x,y) from img
      */
     double sum = 0;
     int index = 0;

     Uint32 pixel;
     Uint8 r,g,b;

    for(int i = -1;i<=1;i++)
    {
	 for(int j = -1;j<=1;j++)
	 {
	      if(x+i<img->w && x+i >= 0 && y+j<img->h && y+j >= 0)
	      {
		   pixel = get_pixel(img,i+x,j+y);
		   SDL_GetRGB(pixel, img->format, &r, &g, &b);
		   sum += r*matr[index];
	      }
	      index += 1;
	 }
    }
    return sum;
}

SDL_Surface *Sobel(SDL_Surface *img, int var)
{
     /*
       apply Sobel filter and inverse result
      */
     Uint32 newpixel;
     int w = img->w;
     int h = img -> h;

     double gx,gy,val;

     SDL_Surface *result = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

     double kernelx[3*3] = {-1.0, 0.0, 1.0,
	  -2.0, 0.0, 2.0,
	  -1.0, 0.0, 1.0};

     double kernely[3*3] = {-1.0,-2.0,-1.0,
	  0.0,0.0,0.0,
	  1.0,2.0,1.0};

     for(int i = 2;i<w-2;i++)
     {
	  for(int j = 2;j<h-2;j++)
	  {
	       gx = convolution(img, kernelx, i,j);
	       gy = convolution(img, kernely, i,j);

	       val = sqrt(gx*gx + gy*gy);
	       if(val <= 128)
		    newpixel = SDL_MapRGB(img->format, 0, 0, 0);
	       else
		    newpixel = SDL_MapRGB(img->format, 255, 255, 255);
	       put_pixel(result, i,j,newpixel);
	  }
     }
     if (var)
         SDL_FreeSurface(img);
     return result;
}
