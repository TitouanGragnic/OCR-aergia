#include "../../include/slot_processing/slicing.h"

void slicing(SDL_Surface* main_surface)
{
    /*
      slices the grid into 81 slots and saves them in output
    */
    SDL_Surface* slot_surface;
    SDL_Surface* edge_surface;
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
            edge_surface = Sobel(slot_surface, 0);
            result_surface = blobSlot(edge_surface, 0, slot_surface);
            result_surface = resize_slot(result_surface);
            file_name[16] = '0'+number/10;
            file_name[17] = '0'+number%10;
            SDL_SaveBMP(result_surface, file_name);
            printf("%d \n", number);
            number++;
	}
    }
    SDL_FreeSurface(result_surface);
    SDL_FreeSurface(main_surface);
}
