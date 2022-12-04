#include "../../include/slot_processing/redraw.h"

void matrixToSurface(int *matrix, SDL_Surface *img, int x, int y, int u, int v)
{
    int neww = u-x;
    int newh = v-y;

    int divW = neww/16;
    int divH = newh/16;

    Uint32 newPixel = SDL_MapRGB(img->format, 0, 0, 0);
    Uint32 white = SDL_MapRGB(img->format, 255, 0, 0);

    for (int i = x;i<u; i++)
    {
        for (int j = y; j<v; j++)
        {
            if (matrix[(i-u)/divW*16+(j-v)/divH] == 1)
                put_pixel(img,i,j,newPixel);
            else
                put_pixel(img, i, j, white);
        }
    }
    free(matrix);
}
