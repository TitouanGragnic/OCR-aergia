#ifndef SLICING_H_
#define SLICING_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"
#include "../utils/init.h"
#include "../utils/resize.h"
#include "../detection/grid_detection.h"
#include "../utils/scale.h"
#include "../color_treatment/edge_detection.h"

void slicing(SDL_Surface* surface);

#endif
