#ifndef RESIZE_H_
#define RESIZE_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "init.h"

SDL_Surface* resize(SDL_Surface* start_surface);


#endif
