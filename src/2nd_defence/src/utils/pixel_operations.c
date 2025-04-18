#include "../../include/utils/pixel_operations.h"

static inline
Uint8* pixel_ref(SDL_Surface* surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface* surface, unsigned x, unsigned y)
{
    Uint8* p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16*)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32*)p;
    }

    return 0;
}

void put_pixel(SDL_Surface* surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8* p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

Uint8 pixel_value( SDL_Surface* img, int x, int y)
{
    /*
      return red value on pixel
    */
    Uint32 pixel;
    pixel = get_pixel(img, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, img->format, &r, &g, &b);
    return r;
}

void put_pixel_value( SDL_Surface* img, int x, int y, int value)
{
    Uint32 pixel = SDL_MapRGB( img->format, value, value, value);
    put_pixel( img, x, y, pixel);
}

SDL_Surface* copy_surface(SDL_Surface* img)
{
    SDL_Surface* out =  SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);

    for (int i = 0; i < img->w; i++)
        for (int j = 0; j <img->h; j++)
            put_pixel(out, i, j, get_pixel(img, i, j));
    return out;
}
