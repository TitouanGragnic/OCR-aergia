#ifndef DRAW_H_
#define DRAW_H_

#include <math.h>
#include "pixel_operations.h"

void dotRed(SDL_Surface* img,int w, int h, int i , int j);
void dotBlue(SDL_Surface* img,int w, int h, int i , int j);
void dotGreen(SDL_Surface* img,int w, int h, int i , int j);
SDL_Surface *ligne_bleu(SDL_Surface* img, int t);
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1);

#endif
