#include "../../include/utils/scale.h"

SDL_Surface *scale(SDL_Surface *start_surface, int x1, int y1, int x2, int y2)
{
    SDL_Surface* scale_surface; //result surface
    Uint8 r,b,g;
    Uint32 pixel;
    Uint32 newPixel;

    //size scale image
    int w = x2-x1+1;
    int h = y2-y1+1;
    //create base for result
    scale_surface = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

    for(int i = 0; i< w; i++)
	 for(int j = 0; j< h; j++)
	      if(x1+i< start_surface->w && y1+j< start_surface->h)
	      {
		  //get pixel on start image
		   pixel = get_pixel(start_surface,x1+i,y1+j);
		   SDL_GetRGB(pixel,start_surface->format,&r,&g,&b);
		   //put on the right coor in scale image
		   newPixel = SDL_MapRGB(scale_surface->format, r, g, b);
		   put_pixel(scale_surface,i,j,newPixel);
	      }
    SDL_FreeSurface(start_surface);
    return scale_surface;
}
