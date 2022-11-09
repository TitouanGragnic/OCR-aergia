#include "../../include/color_treatment/blur.h"

SDL_Surface *blur(SDL_Surface *img)
{
     /*
       blur filter
      */
     SDL_Surface *oi = SDL_CreateRGBSurface(0,img->w,img->h,32,0,0,0,0);
     Uint8 r,g,b;
     Uint32 pixel;

     float *matr = malloc(9*sizeof(float));
     float pascal[3*3] = {
	  1.0/16.0 , 2.0/16.0, 1.0/16.0,
	  2.0/16.0 , 4.0/16.0, 2.0/16.0,
	  1.0/16.0 , 2.0/16.0, 1.0/16.0
     };
     float result = 0;

     int index = 0;

     for(int ix=0; ix < img->w; ix++) {
	  for(int iy=0; iy < img->h; iy++) {
	       index = 0;
	       for(int kx=-1; kx <= 1; kx++) {
		    for(int ky=-1; ky <= 1; ky++) {
			 if(ix+kx>=0 && ix+kx<img->w && iy+ky>=0 && iy+ky<img->h){
			      pixel = get_pixel(img, ix+kx,iy+ky);
			      SDL_GetRGB(pixel, img->format, &r, &g, &b);
			      matr[index] = r;
			 }
			 else
			      matr[index] = 0;
			 index += 1;
		    }
	       }
	       result = 0;
	       for(int i = 0;i<9;i++)
	       {
		    result += (pascal[i]*matr[i]);
	       }
	       pixel = SDL_MapRGB(img->format, result, result, result);
	       put_pixel(oi, ix, iy, pixel);
	  }
     }
     free(matr);
     SDL_FreeSurface(img);
     return oi;
}
