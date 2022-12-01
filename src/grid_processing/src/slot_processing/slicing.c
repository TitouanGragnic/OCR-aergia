#include "../../include/slot_processing/slicing.h"

void slicing(SDL_Surface* main_surface)
{
    /*
      slices the grid into 81 slots and saves them in output
    */
    SDL_Surface* slot_surface;
    SDL_Surface* copy;
    SDL_Surface* result_surface;

    int number = 0;
    char file_name[] = "output/slot/slot00.png";

    Uint32 pixel;
    Uint32 newPixel;

    int w = main_surface->w;
    int h = main_surface->h;
    int newW = w/9;//here to change the number of grid per row
    int newH = h/9;//per lines

    Uint8 r,g,b;

    for (int i = 0;i<9;i++)
    {
        for (int j = 0;j<9;j++)
	{
            slot_surface = SDL_CreateRGBSurface(0, newW, newH,32,0,0,0,0);
	    copy = SDL_CreateRGBSurface(0, newW, newH,32,0,0,0,0);
            for(int x = 0;x<newW;x++)
	    {
                for(int y = 0;y<newH;y++)
		{
                    pixel = get_pixel(main_surface,i*newW+x,j*newH+y);
                    SDL_GetRGB(pixel,slot_surface->format,&r,&g,&b);
                    //put on the right color
                    newPixel = SDL_MapRGB(main_surface->format, r, g, b);
                    put_pixel(slot_surface,x,y,newPixel);
		    put_pixel(copy,x,y,newPixel);
		}
	    }
	    result_surface = resize_slot(copy);
	    slot_surface = resize_slot(slot_surface);
	    for (int x = 0;x<16;x++)
	    {
		 for (int y = 0; y<3; y++)
		 {
		      newPixel = SDL_MapRGB(main_surface->format, 255, 255, 255);
		      put_pixel(result_surface,x,y,newPixel);
		 }
		 for (int y = 13;y<16;y++)
		 {
		      newPixel = SDL_MapRGB(main_surface->format, 255, 255, 255);
		      put_pixel(result_surface,x,y,newPixel);
		 }
	    }
	    for (int y = 2;y<14;y++)
	    {
		 for (int x = 0;x<3; x++)
		 {
		      newPixel = SDL_MapRGB(main_surface->format, 255, 255, 255);
		      put_pixel(result_surface,x,y,newPixel);
		 }
		 for (int x = 13;x<16;x++)
		 {
		      newPixel = SDL_MapRGB(main_surface->format, 255, 255, 255);
		      put_pixel(result_surface,x,y,newPixel);
		 }
	    }
            slot_surface = blobSlot(result_surface, 0, slot_surface);
            
            file_name[16] = '0'+number/10;
            file_name[17] = '0'+number%10;
            SDL_SaveBMP(slot_surface, file_name);
            printf("%d \n", number);
            number++;
	}
    }
    SDL_FreeSurface(slot_surface);
    SDL_FreeSurface(main_surface);
}
