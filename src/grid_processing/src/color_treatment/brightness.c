#include "../../include/color_treatment/brightness.h"


void brightness(SDL_Surface *img, double max)
{
     /*
       equalize the brightness
      */
     int h = img->h;
     int w = img->w;

     Uint32 pixel;
     Uint32 newpixel;
     Uint8 r, g, b;

     double val;

     for(int i = 0; i<w;i++)
	  for(int j=0; j<h;j++)
	  {
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       val = 255-(double)r*(255.0/max);
	       newpixel = SDL_MapRGB(img->format, val, val, val);
	       put_pixel(img, i, j, newpixel);
	  }
}
