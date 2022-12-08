#include "../../include/utils/rotation.h"
#define M_PI       3.14159265358979323846

SDL_Surface *rotate(SDL_Surface *img, double alpha, int var)
{
    int w = img->w;
    int h = img->h;
    //convert angle from degree to radiant
    alpha *= (M_PI/180);
    //calculation of the diagonale
    int newLong = (int)sqrt(h*h+w*w)+2;
    SDL_Surface *result = SDL_CreateRGBSurface(0,newLong,newLong,32,0,0,0,0);
    if(var)
        for(int i=0;i<newLong;i++)
            for(int j=0;j<newLong;j++)
                put_pixel_value(result, i, j, 255);

    //init value to calculate new coordonate
    int midx = w/2;
    int midy = h/2;
    int mid = newLong/2;
    for(int y = 0;y<h;y++){
        for(int x = 0;x<w;x++){
            int i = (x-midx)*cos(alpha)+(y-midy)*sin(alpha)+mid;//calculation of new coordonee
            int j = -(x-midx)*sin(alpha)+(y-midy)*cos(alpha)+mid;
            //put pixel value in new location
            put_pixel_value(result, i, j, pixel_value(img, x, y));
        }
    }
    SDL_FreeSurface(img);//free memory
    return result;
}
