#ifndef ROTATION_H_
#define ROTATION_H_

#include <math.h>
#include "pixel_operations.h"

#define M_PI       3.14159265358979323846

struct line{
     double x1;
     double y1;
     double x2;
     double y2;
};

SDL_Surface *rotate(SDL_Surface *img, double alpha, struct line *line, int len,int var);

#endif
