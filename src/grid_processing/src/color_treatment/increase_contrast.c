#include "../../include/color_treatment/increase_contrast.h"

int increase_contrast(SDL_Surface* img, double fact)
{
     /*
       increase contrast in img
      */
     int h = img->h;
     int w = img->w;

     Uint32 pixel;
     Uint32 newpixel;
     Uint8 r, g, b;
     double val;
     int max = 0;

     for(int i = 0; i<w;i++)
	  for(int j=0; j<h;j++)
	  {
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       newpixel = SDL_MapRGB(img->format, 255-r, 255-g, 255-b);

	       for(int v = 0; v<fact; v++)
	       {
		    if(r>=v*(255/fact) && r<=(v+1)*(255/fact))
		    {
			 val = (v+1)*(255.0/fact);
			 newpixel = SDL_MapRGB(img->format, 255-val,255-val, 255-val);
		    }
	       }
	       SDL_GetRGB(newpixel, img->format, &r, &g, &b);
	       if(r > max)
		    max = r;
	       put_pixel(img, i, j, newpixel);
	  }
     return max;
}
