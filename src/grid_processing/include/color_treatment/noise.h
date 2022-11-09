#ifndef NOISE_H_
#define NOISE_H_

#include "../utils/pixel_operations.h"

SDL_Surface *reduce_noise(SDL_Surface *img);
int noise(SDL_Surface* img, int w, int h);

#endif
