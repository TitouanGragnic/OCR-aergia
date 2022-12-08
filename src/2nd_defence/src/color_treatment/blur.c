#include "../../include/color_treatment/blur.h"

SDL_Surface *blur(SDL_Surface *img)
{
    /*
      blur filter
    */
    SDL_Surface *oi = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
    //Uint8 r,g,b;
    //Uint32 pixel;

    float *matr = malloc(9 * sizeof(float));
    float pascal[3*3] =
        {
            1.0/16.0 , 2.0/16.0, 1.0/16.0,
            2.0/16.0 , 4.0/16.0, 2.0/16.0,
            1.0/16.0 , 2.0/16.0, 1.0/16.0
        };

    float result = 0;
    int index = 0;

    for (int i = 0; i < img->w; i++)
    {
        for (int j = 0; j < img->h; j++)
        {
            index = 0;
            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    if (i+ki >=0 && i+ki < img->w && j+kj >= 0 && j+kj < img->h)
                        matr[index] = pixel_value(img, i + ki, j + kj);
                    else
                        matr[index] = 0;
                    index += 1;
                }
            }
            result = 0;
            for(int i = 0;i<9;i++)
                result += (pascal[i]*matr[i]);

            put_pixel_value(oi, i, j, result);
        }
    }
    free(matr);
    SDL_FreeSurface(img);
    return oi;
}
