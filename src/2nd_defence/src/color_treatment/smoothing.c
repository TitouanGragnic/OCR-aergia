#include "../../include/color_treatment/smoothing.h"

SDL_Surface *smoothing(SDL_Surface *img)
{
    /*
      smooth the img
    */
    int w = img->w;
    int h = img->h;

    SDL_Surface *result = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);


    int count = 0;


    for(int i = 0; i<w; i++)
    {
        for(int j = 0; j<h; j++)
        {
            int value = pixel_value( img, i, j);
            if(value == 0)
            {
                count = 0;
                for(int kx=-1; kx <= 1 && count == 0; kx++) {
                    for(int ky=-1; ky <= 1 && count == 0; ky++) {
                        if(i+kx>=0 && i+kx<img->w && j+ky>=0 && j+ky<img->h){
                            if(pixel_value( img, i + kx, j + ky) == 255)
                                count += 1;
                        }
                    }
                }
                if(count > 0)
                    put_pixel_value(result, i, j, 255);
            }
            else
                put_pixel_value(result, i, j, value);
        }
    }
    SDL_FreeSurface(img);
    return result;
}
