#ifndef PIXEL_FOR_NEURAL_H_
#define PIXEL_FOR_NEURAL_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
Uint8 pixelValue(int x, int y, SDL_Surface* img);

#endif
