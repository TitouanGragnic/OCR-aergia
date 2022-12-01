#include "../../include/color_treatment/increase_contrast.h"

int increase_contrast(SDL_Surface* img, double fact)
{
    /*
      increase contrast in img
    */
    int h = img->h;
    int w = img->w;
    int max = 0;

    for(int i = 0; i<w;i++)
    {
        for(int j=0; j<h;j++)
        {
            int color = pixel_value(img, i, j);
            for(int v = 0; v<fact; v++)
            {
                if(color >= v * 255.0 / fact && color <= (v + 1.0) * 255.0 / fact)
                    color = (v + 1) * 255.0 / fact;
            }
            color = 255 - color;
            if(color > max)
                max = color;
            put_pixel_value(img, i, j, color);
        }
    }
    return max;
}
