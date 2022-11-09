#include "../../include/color_treatment/smoothing.h"

SDL_Surface *smoothing(SDL_Surface *img)
{
     /*
       smooth the img
      */
     int w = img->w;
     int h = img->h;

     SDL_Surface *result = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

     Uint32 pixel;
     Uint32 newpixel;
     Uint8 r,g,b;

     int count = 0;

     newpixel = SDL_MapRGB(img->format, 255, 255, 255);

     for(int i = 0; i<w; i++)
     {
	  for(int j = 0; j<h; j++)
	  {
	       pixel = get_pixel(img, i, j);
	       SDL_GetRGB(pixel, img->format, &r, &g, &b);
	       if(r == 0)
	       {
		    count = 0;
		    for(int kx=-1; kx <= 1 && count == 0; kx++) {
			 for(int ky=-1; ky <= 1 && count == 0; ky++) {
			      if(i+kx>=0 && i+kx<img->w && j+ky>=0 && j+ky<img->h){
				   pixel = get_pixel(img, i+kx, j+ky);
				   SDL_GetRGB(pixel, img->format, &r, &g, &b);
				   if(r == 255)
					count += 1;
			      }
			 }
		    }
		    if(count > 0)
			 put_pixel(result, i, j, newpixel);
	       }
	       else
		    put_pixel(result, i, j, pixel);
	  }
     }
     SDL_FreeSurface(img);
     return result;
}
