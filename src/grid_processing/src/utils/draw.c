#include "../../include/utils/draw.h"

#define M_PI       3.14159265358979323846

void dotRed(SDL_Surface* img,int w, int h, int i , int j)
{
    Uint32 newpixel;
    int s = 2;
    for(int ki = -s; ki<=s; ki++){
	for(int kj = -s; kj<=s; kj++){
	    if((j+kj)<h && (ki+i)<w && j+kj>=0 && i+ki>=0 ){
		newpixel = SDL_MapRGB(img->format, 255, 0, 0);
		put_pixel(img,i+ki,j+kj,newpixel);
	    }
	}
    }
}

void dotBlue(SDL_Surface* img,int w, int h, int i , int j)
{
    Uint32 newpixel;
    int s = 2;
    for(int ki = -s; ki<=s; ki++){
	for(int kj = -s; kj<=s; kj++){
	    if((j+kj)<h && (ki+i)<w && j+kj>=0 && i+ki>=0 ){
		newpixel = SDL_MapRGB(img->format, 0, 0, 255);
		put_pixel(img,i+ki,j+kj,newpixel);
	    }
	}
    }
}

void dotGreen(SDL_Surface* img,int w, int h, int i , int j)
{
    Uint32 newpixel;
    int s = 20;
    for(int ki = -s; ki<=s; ki++){
	for(int kj = -s; kj<=s; kj++){
	    if((j+kj)<h && (ki+i)<w && j+kj>=0 && i+ki>=0 ){
		newpixel = SDL_MapRGB(img->format, 0, 255, 0);
		put_pixel(img,i+ki,j+kj,newpixel);
	    }
	}
    }
}

void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1)
{
     Uint32 pixel = SDL_MapRGB(Screen->format, 255, 0, 0);
     int i;
     double x = (x1 - x0);
     double y = (y1 - y0);
     double length = sqrt( x*x + y*y );
     double addx = x / length;
     double addy = y / length;
     x = x0;
     y = y0;

     for ( i = 0; i < length; i += 1) {
	  if(x>=0 && y>=0 && y<Screen->h && x<Screen->w)
	       put_pixel(Screen, x, y, pixel );
	  x += addx;
	  y += addy;
     }
}

SDL_Surface *ligne_bleu(SDL_Surface* img, int t)
{
     int w= img->w ;
     int h = img->h;
     int j;
     SDL_Surface *img1 = img;
     for(int i =0 ;i<w;i++){
	  double theta = (double)t*(M_PI/180);
	  double k = -cos(theta)/sin(theta);
	  double temp_p = sqrt(w*w+h*h)/(3*sin(theta));
	  j =(int)(k*(double)i+temp_p) ;
	  if(j<h && j>=0 ){
	       dotBlue(img1,w,h,i,j);
	  }
     }
     return img1;
}
