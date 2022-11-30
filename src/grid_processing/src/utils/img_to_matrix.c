#include "../../include/utils/img_to_matrix.h"

int** img_to_matrix(SDL_Surface* img)
{
    int ** res = malloc(img->w*sizeof(int*));
    for(int i = 0; i < img->w; i++)
    {
        res[i] = malloc(img->h*sizeof(int));
        for(int j =0; j < img->h; j++)
        {
            if (pixelValue( i, j, img) == 255)
                res[i][j] = 1;
            else
                res[i][j] = 0;
        }
    }
    return res;
}
