#include "../../include/utils/rotation.h"

#define M_PI       3.14159265358979323846



SDL_Surface *rotate(SDL_Surface *img, double alpha, struct line *line, int len, int var)
{
     //init future pixel, width end height
     Uint32 pixel;
     int w = img->w;
     int h = img->h;
     //convert angle from degree to radiant
     alpha *= (M_PI/180);
     //calculation of the diagonale
     int newLong = (int)sqrt(h*h+w*w)+2;
     SDL_Surface *result = SDL_CreateRGBSurface(0,newLong,newLong,32,0,0,0,0);
     if(var)
     {
	  for(int i=0;i<newLong;i++)
	  {
	       for(int j=0;j<newLong;j++)
	       {
		    pixel = SDL_MapRGB(img->format, 255, 255, 255);
		    put_pixel(result,i,j,pixel);
	       }
	  }
     }

     //init value to calculate new coordonate
     int midx = w/2;
     int midy = h/2;
     int mid = newLong/2;

     for(int y = 0;y<h;y++){
	  for(int x = 0;x<w;x++){
	       int i = (x-midx)*cos(alpha)+(y-midy)*sin(alpha)+mid;//calculation of new coordonee
	       int j = -(x-midx)*sin(alpha)+(y-midy)*cos(alpha)+mid;
	       pixel = get_pixel(img,x,y);//get pixel from old location
	       put_pixel(result,i,j,pixel);//put pixel in new location
	  }
     }
     for(int i = 0;i<len;i++)//rotate list of line too
       {
       int x1 = line[i].x1;
       int x2 = line[i].x2;
       int y1 = line[i].y1;
       int y2 = line[i].y2;

       line[i].x1 = (x1-midx)*cos(alpha)+(y1-midy)*sin(alpha)+mid;
       line[i].x2 = (x2-midx)*cos(alpha)+(y2-midy)*sin(alpha)+mid;
       line[i].y1 = -(x1-midx)*sin(alpha)+(y1-midy)*cos(alpha)+mid;
       line[i].y2 = -(x2-midx)*sin(alpha)+(y2-midy)*cos(alpha)+mid;
       }
     SDL_FreeSurface(img);//free memory
     return result;
}