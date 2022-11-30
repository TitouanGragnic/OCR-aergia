#ifndef RESIZE_H_
#define RESIZE_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../utils/pixel_operations.h"
#include "../utils/init.h"

SDL_Surface* resize(SDL_Surface* start_surface);
SDL_Surface* resize_slot(SDL_Surface* start_surface);

#endif
