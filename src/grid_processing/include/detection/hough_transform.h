#ifndef HOUGH_TRANSFORM_H_
#define HOUGH_TRANSFORM_H_

#include "../utils/rotation.h"
#include "../utils/draw.h"
#include "../utils/linked_list.h"

SDL_Surface* hough_transform_rotate(SDL_Surface* edge_surface,SDL_Surface **bin_surface);
#endif
