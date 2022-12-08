#include "../../include/color_treatment/inverse.h"

SDL_Surface *inverse_color(SDL_Surface *img)
{
    int w = img->w;
    int h = img->h;

    int max = 0;

    Uint8 r,g,b;
    Uint32 pixel;
    Uint32 newpixel;

    SDL_Surface *inverse = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

    for (int i = 0; i<w; i++)
    {
        for (int j = 0; j<h; j++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            newpixel = SDL_MapRGB(inverse->format, 255-r, 255-g, 255-b);
            put_pixel(inverse, i, j, newpixel);
            max += (0.33*r+0.33*g+0.33*b);
        }
    }
    max = max/(w*h);
    if (max >= 128)
    {
        SDL_FreeSurface(inverse);
        return img;
    }
    SDL_FreeSurface(img);
    return inverse;
}
