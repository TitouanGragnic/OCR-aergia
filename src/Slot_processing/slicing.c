#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "init.h"

void slicing(SDL_Surface* main_surface)
{
  /*
    slices the grid into 81 slots and saves them in output
   */
  SDL_Surface* slot_surface;

  int number = 0;
  char file_name[] = "output/slot00.png";

  Uint32 pixel;
  Uint32 newPixel;
  
  int w = main_surface->w;
  int h = main_surface->h;
  int newW = w/9;//here to change the number of grid per row
  int newH = h/9;//per lines

  Uint8 r,g,b;
  
  slot_surface = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);
  
  for (int i = 0;i<9;i++)
    {
      for (int j = 0;j<9;j++)
	{
	  for(int x = 0;x<newW;x++)
	    {
	      for(int y = 0;y<newH;y++)
		{
		  pixel = get_pixel(main_surface,i*newW+x,j*newH+y);
		  SDL_GetRGB(pixel,slot_surface->format,&r,&g,&b);
		  //put on the right color
		  newPixel = SDL_MapRGB(main_surface->format, r, g, b);
		  put_pixel(slot_surface,x,y,newPixel);
		}
	    }
	  file_name[11] = '0'+number/10;
	  file_name[12] = '0'+number%10;
	  SDL_SaveBMP(slot_surface, file_name);
	  number++;
	}
    }
  SDL_FreeSurface(slot_surface);
  SDL_FreeSurface(main_surface);
}
