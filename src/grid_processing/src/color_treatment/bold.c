#include "../../include/color_treatment/bold.h"

SDL_Surface *bold(SDL_Surface *im, int pui)
{
    //bold black pixel on image
    SDL_Surface *oi;
    Uint8 r,g,b;
    Uint32 newpixel;
    Uint32 pixel;
    newpixel = SDL_MapRGB(im->format, 0, 0, 0);
    oi = SDL_CreateRGBSurface(0,im->w,im->h,32,0,0,0,0);

    for(int ix=0; ix < im->w; ix++) {
	for(int iy=0; iy < im->h; iy++) {
	    pixel = get_pixel(im, ix, iy);
	    SDL_GetRGB(pixel, im->format, &r, &g, &b);
	    if(r == 0){
		for(int kx=-pui; kx <= pui; kx++) {
		    for(int ky=-pui; ky <= pui; ky++) {
			if(ix+kx>=0 && ix+kx<im->w && iy+ky>=0 && iy+ky<im->h){
			    put_pixel(oi,ix+kx,iy+ky,newpixel);
			}
		    }
		}
	    }
	    else
		put_pixel(oi,ix,iy,pixel);
	}
    }
    SDL_FreeSurface(im);
    return oi;
}
