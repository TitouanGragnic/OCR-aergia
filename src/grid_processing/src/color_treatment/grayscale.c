#include "../../include/color_treatment/grayscale.h"

void grayscale(SDL_Surface* img)
{
    /*
      convert to grey image form basic image
    */
    Uint32 pixel;
    Uint8 r, g, b;
    Uint8 average;

    for(int y = 0; y< img->h; y++)
    {
        for(int x = 0; x< img->w; x++)
        {
            pixel = get_pixel(img, x, y);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            average = 0.3*r + 0.59*g + 0.11*b;
            put_pixel_value(img, x, y, average);
        }
    }
}
