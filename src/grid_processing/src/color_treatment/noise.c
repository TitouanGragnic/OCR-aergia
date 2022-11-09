#include "../../include/color_treatment/noise.h"

void sort(int* tab,int len)
{
     /*
       quick sort
     */
     if(len < 2)
	  return;

     int pivot = tab[len / 2];

     int i, j;
     for (i = 0, j = len - 1; ; i++, j--)
     {
	  while(tab[i] < pivot)
	       i++;
	  while(tab[j] > pivot)
	       j--;

	  if(i >= j)
	       break;

	  int temp = tab[i];
	  tab[i] = tab[j];
	  tab[j] = temp;
     }

  sort(tab, i);
  sort(tab + i, len - i);
}

SDL_Surface *reduce_noise(SDL_Surface *img)
{
     /*
       apply filter to reduce noise of img
      */
    SDL_Surface *oi = SDL_CreateRGBSurface(0,img->w,img->h,32,0,0,0,0);
    Uint8 r,g,b;
    Uint32 pixel;
    int *matr = malloc(9*sizeof(int));
    int index = 0;

    for(int ix=0; ix < img->w; ix++) {
	for(int iy=0; iy < img->h; iy++) {
	     index = 0;
	     for(int kx=-1; kx <= 1; kx++) {
		  for(int ky=-1; ky <= 1; ky++) {
		       if(ix+kx>=0 && ix+kx<img->w && iy+ky>=0 && iy+ky<img->h){
			    pixel = get_pixel(img, ix+kx,iy+ky);
			    SDL_GetRGB(pixel, img->format, &r, &g, &b);
			    matr[index] = r;
		       }
		       else
			    matr[index] = 0;
		       index += 1;
		  }
	     }
	     sort(matr,9);
	     pixel = SDL_MapRGB(img->format, matr[4], matr[4], matr[4]);
	     put_pixel(oi, ix, iy, pixel);
	}
    }
    free(matr);
    SDL_FreeSurface(img);
    return oi;
}

int noise(SDL_Surface* img, int w, int h)
{
     /*
       return noise level on image done
     */
     double sum = 0;
     Uint8 r, g, b;
     int count = 0;
     double val = 0;
     Uint32 pixel;
     for(int i = 0; i<w;i++)
	  for(int j = 0; j<h; j++)
	  {
	       sum=0;
	       for(int kx=-1; kx <= 1; kx++)
		    for(int ky=-1; ky <= 1; ky++)
			 if(i+kx>=0 && i+kx<img->w && j+ky>=0 && j+ky<img->h)
			 {
			      pixel = get_pixel(img, i+kx, j+ky);
			      SDL_GetRGB(pixel, img->format, &r, &g, &b);
			      sum += r;
			 }
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       sum = sum/9.0;
	       val = 1-((double)r/sum);
	       if(val < 0)
		    val *= -1;
	       if(val > 0.5)
		    count += 1;
	  }
     return count;
}
