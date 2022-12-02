#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <SDL/SDL.h>
#include <err.h>

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
Uint8 pixel_value( SDL_Surface* img, int x, int y);
void put_pixel_value( SDL_Surface* img, int x, int y, int value);
SDL_Surface* copy_surface(SDL_Surface* img);

#endif
