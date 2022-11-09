#ifndef INIT_H_
#define INIT_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void init_sdl();
SDL_Surface* load_image(char* path);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
SDL_Surface* display_image(SDL_Surface* img);
void wait_for_keypressed();
void SDL_FreeSurface(SDL_Surface *surface);

#endif
