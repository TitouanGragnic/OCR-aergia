#include "../../include/color_treatment/adaptive_threshold.h"

void adaptive_threshold(SDL_Surface *img, double t, int detail)
{
     /*
       from img to binarize
      */
     Uint32 pixel;
     Uint8 r,g,b;

     int w = img->w;
     int h = img->h;

     int s2 = fmax(w,h) / detail;
     unsigned long *all_image = calloc(w*h,sizeof(unsigned long));
     long sum = 0;
     unsigned int count = 0;
     int x1,y1,x2,y2;

     for(int j = 0; j<h; j++)
     {
	  pixel = get_pixel(img,0,j);
	  SDL_GetRGB(pixel, img->format, &r,&g,&b);
	  sum += r;
	  all_image[j] = sum;
     }

     for(int i = 1; i<w; i++)
     {
	  sum = 0;
	  for(int j = 0; j<h; j++)
	  {
	       pixel = get_pixel(img,i,j);
	       SDL_GetRGB(pixel, img->format, &r,&g,&b);
	       sum += r;
	       all_image[i*h+j] = all_image[(i-1)*h + j] + sum;
	  }
     }

     for(int i = 0; i<w; i++)
     {
	  for(int j = 0; j<h; j++)
	  {
	       pixel = get_pixel(img,i,j);
	       SDL_GetRGB(pixel, img->format, &r,&g,&b);

	       x1 = fmax(i-s2, 1);
	       x2 = fmin(i+s2, w-1);
	       y1 = fmax(j-s2, 1);
	       y2 = fmin(j+s2, h-1);
	       count = (x2-x1) * (y2-y1);

	       sum = all_image[x2*h+y2]
		    -all_image[x2*h+(y1-1)]
		    -all_image[(x1-1)*h+y2]
		    +all_image[(x1-1)*h+(y1-1)];

	       if(r*count < sum*(1.0-t))
		    pixel = SDL_MapRGB(img->format, 0, 0, 0);
	       else
		    pixel = SDL_MapRGB(img->format, 255, 255, 255);

	       put_pixel(img, i, j, pixel);
	  }
     }
     free(all_image);
}
