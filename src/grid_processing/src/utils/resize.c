#include "../../include/utils/resize.h"


Uint32 get_average_color(SDL_Surface* start_surface,int x,int y,int s, SDL_PixelFormat *format )
{
    Uint8 r,g,b;
    int resR = 0;
    int resG = 0;
    int resB = 0;
    Uint32 pixel;
    for(int i = x;i<x+s && i<start_surface->w;i++)
        for(int j = y;j<y+s && j<start_surface->w;j++)
        {
            pixel = get_pixel(start_surface,i,j);
            SDL_GetRGB(pixel,start_surface->format,&r,&g,&b);
            resR += r;
            resG += g;
            resB += b;
        }
    return SDL_MapRGB(format, resR/(s*s), resG/(s*s), resB/(s*s));

}

SDL_Surface* resize(SDL_Surface* start_surface)
{
    SDL_Surface* resize_surface;
    int w = start_surface->w;
    int h = start_surface->h;
    double s;

    if(w>h)
        s = (double)w/1024;
    else
        s = (double)h/1024;

    int newW = w/s ;
    int newH = h/s ;
    Uint32 newPixel;

    resize_surface = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);
    for(int i = 0; i< newW; i++)
        for(int j = 0; j< newH; j++)
        {
            newPixel =get_average_color(start_surface,i*s,j*s,s,resize_surface->format);
            put_pixel(resize_surface,i,j,newPixel);

        }

    SDL_FreeSurface(start_surface);
    return resize_surface ;
}

SDL_Surface* resize_slot(SDL_Surface* start_surface)
{
    SDL_Surface* resize_surface;
    int w = start_surface->w;
    int h = start_surface->h;
    double s;
    Uint8 r,g,b;
    if(w>h)
        s = (double)w/16;
    else
        s = (double)h/16;
    /*int newW = w/s ;
      int newH = h/s ;*/
    Uint32 newPixel;
    resize_surface = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);

    for(int i = 0; i< 16; i++)
        for(int j = 0; j< 16; j++)
        {
            newPixel =get_average_color(start_surface,i*s,j*s,s,start_surface->format);
            SDL_GetRGB(newPixel,start_surface->format,&r,&g,&b);
            if(0.33*r+0.33*g+0.33*b < 150)
                newPixel = SDL_MapRGB(resize_surface->format, 0, 0, 0);
            else
                newPixel = SDL_MapRGB(resize_surface->format, 255, 255, 255);
            put_pixel(resize_surface,i,j,newPixel);
        }

    SDL_FreeSurface(start_surface);
    return resize_surface ;
}

SDL_Surface* resize_slot2(SDL_Surface* start_surface, int x, int y1, int y2)
{
    SDL_Surface* resize_surface;
    //int w = start_surface->w;
    //int h = start_surface->h;
    double s;
    Uint8 r,g,b;
    double w = y2-y1;

    int x1 = x - w/2;
    if(x1<0)
        x1 = 0;
    //int x2 = x + w/2;
    s = w/16.0;
    Uint32 newPixel;
    resize_surface = SDL_CreateRGBSurface(0,16,16,32,0,0,0,0);

    for(int i = 0; i< 16; i++)
        for(int j = 0; j< 16; j++)
        {
            if(i == 0 || j ==0 || i == 15 || j == 15)
                put_pixel_value(resize_surface,i,j,255);
            else
            {
                newPixel =get_average_color(start_surface,x1+i*s,y1+j*s,s,start_surface->format);
                SDL_GetRGB(newPixel,start_surface->format,&r,&g,&b);
                if(0.33*r+0.33*g+0.33*b < 150)
                    newPixel = SDL_MapRGB(resize_surface->format, 0, 0, 0);
                else
                    newPixel = SDL_MapRGB(resize_surface->format, 255, 255, 255);
                put_pixel(resize_surface,i,j,newPixel);
            }
        }

    SDL_FreeSurface(start_surface);
    return resize_surface ;
}
