#include "../../include/color_treatment/brightness.h"

void brightness(SDL_Surface *img, double max)
{
    /*
      equalize the brightness
    */
    int h = img->h;
    int w = img->w;

    double val;
    double color;

    for (int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            color = pixel_value(img, i, j);
            val = 255.0 - color * 255.0 / max;
            put_pixel_value(img, i, j, val);
        }
    }
}
