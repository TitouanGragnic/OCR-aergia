#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "include/utils/init.h"
#include "include/utils/scale.h"
#include "include/utils/resize.h"

#include "include/color_treatment/smoothing.h"
#include "include/color_treatment/increase_contrast.h"
#include "include/color_treatment/blur.h"
#include "include/color_treatment/noise.h"
#include "include/color_treatment/grayscale.h"
#include "include/color_treatment/brightness.h"
#include "include/color_treatment/adaptive_threshold.h"
#include "include/color_treatment/edge_detection.h"

#include "include/detection/grid_detection.h"
#include "include/detection/hough_transform.h"
#include "include/detection/distorsion.h"

struct carth{
    int x;
    int y;
};



int main(int argc, char *argv[])
{
     //read file
     if(argc != 2)
	  errx(1, "Type wanted is : ./out file.png\n");

     SDL_Surface* edge_surface;
     SDL_Surface* bin_surface;
     SDL_Surface* screen_surface;
     int max;

     // : Initialize the SDL
     init_sdl();

     edge_surface = load_image(argv[1]);

     if(edge_surface->w > 1024 ||edge_surface->h > 1024)
	  edge_surface = resize(edge_surface);

     // : Display the image.
     grayscale(edge_surface);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     max = increase_contrast(edge_surface,10);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     brightness(edge_surface,max);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     edge_surface = reduce_noise(edge_surface);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     edge_surface = blur(edge_surface);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     int noise_level = noise(edge_surface, edge_surface->w,edge_surface->h);
     //printf("%d \n",noise);
     adaptive_threshold(edge_surface, noise_level > 300 ? 0.5 : 0.15,32);

     //edge_surface = lissage(edge_surface);
     SDL_SaveBMP(edge_surface, "output/binary.png");

     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     edge_surface = Sobel(edge_surface);
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     bin_surface = load_image("output/binary.png");

     SDL_Surface** bin_surfaceP = &bin_surface;
     edge_surface = hough_transform_rotate(edge_surface,bin_surfaceP);
     bin_surface = *bin_surfaceP;

     printf("process finish\n");
     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     struct blob *blobs = malloc(0*sizeof(struct blob));
     struct blob **B = &blobs;

     struct blob result = blobFromImage(edge_surface, B);
     blobs = *B;

     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     edge_surface = scale(edge_surface, result.min_x, result.min_y, result.max_x,result.max_y);
     bin_surface = scale(bin_surface, result.min_x, result.min_y, result.max_x,result.max_y);

     screen_surface = display_image(edge_surface);
     //wait_for_keypressed();

     screen_surface = display_image(bin_surface);
     //wait_for_keypressed();

     bin_surface = correct_perspective(edge_surface, bin_surface);
     screen_surface = display_image(bin_surface);
     printf("detection finish\n");
     wait_for_keypressed();

    // : Free the image surface.
    // : Free the screen surface.
    SDL_FreeSurface(bin_surface);
    SDL_FreeSurface(edge_surface);
    SDL_FreeSurface(screen_surface);

    free(blobs);
    return 0;
}
