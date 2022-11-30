#ifndef GRID_DETECTION_H_
#define GRID_DETECTION_H_

#include "../utils/pixel_operations.h"
#include "../utils/init.h"

struct blob
{
     int min_x,max_x;
     int min_y,max_y;
     int cen_x,cen_y;
     int n_pixels;
     int ID;
};
struct coor
{
    int x,y;
};
struct blob blobFromImage(SDL_Surface *img, struct blob **blobs, int size);

#endif
