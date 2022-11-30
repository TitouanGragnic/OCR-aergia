#ifndef GRID_DETECTION_H_
#define GRID_DETECTION_H_

#include "../utils/pixel_operations.h"
#include "../utils/init.h"
#include "../utils/stack.h"
#include "../utils/linked_list.h"

typedef struct{
    int min_x,max_x;
    int min_y,max_y;
    int cen_x,cen_y;
    int n_pixels;
    int ID;
}blob;


blob blobFromImage(SDL_Surface *img);

#endif
