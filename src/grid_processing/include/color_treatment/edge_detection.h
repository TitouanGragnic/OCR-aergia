#ifndef EDGE_DETECTION_H_
#define EDGE_DETECTION_H_

#include <math.h>
#include "../utils/pixel_operations.h"
#include "../utils/init.h"

int get_new_value(SDL_Surface *img,int i, int j);
double convolution(SDL_Surface *img , double matr[3*3], int x, int y);
SDL_Surface *Sobel(SDL_Surface *img, int var);

#endif
