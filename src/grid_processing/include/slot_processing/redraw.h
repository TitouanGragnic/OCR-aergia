#ifndef REDRAW_H_
#define REDRAW_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"
#include "../utils/init.h"
#include "../detection/grid_detection.h"



void matrixToSurface(int *matrix, SDL_Surface *img, int x, int y, int u, int v);

#endif
