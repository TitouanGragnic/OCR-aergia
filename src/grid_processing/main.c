#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "include/utils/init.h"
#include "include/utils/scale.h"
#include "include/utils/resize.h"
#include "include/utils/thread.h"

#include "include/color_treatment/smoothing.h"
#include "include/color_treatment/increase_contrast.h"
#include "include/color_treatment/blur.h"
#include "include/color_treatment/noise.h"
#include "include/color_treatment/grayscale.h"
#include "include/color_treatment/brightness.h"
#include "include/color_treatment/adaptive_threshold.h"
#include "include/color_treatment/edge_detection.h"
#include "include/color_treatment/inverse.h"
#include "include/color_treatment/bold.h"

#include "include/detection/grid_detection.h"
#include "include/detection/hough_transform.h"
#include "include/detection/distorsion.h"

#include "include/slot_processing/slicing.h"

#include "include/neural_network/neural_network.h"

#include "include/solver/solver.h"

struct carth{
    int x;
    int y;
};



int main(int argc, char *argv[])
{
    // ----------------------Assert---------------------------------------------
    if(argc > 3 || argc == 1)
        errx(1, "Type wanted is : ./out file.png dev_mod\n");

    size_t NUM_THREADS = 12*2;

    img_thread out;
    out.th = 0;
    out.threads = malloc(sizeof(pthread_t)*NUM_THREADS);
    out.out = malloc(sizeof(img_file)*NUM_THREADS);

    SDL_Surface* edge_surface;
    SDL_Surface* bin_surface;
    SDL_Surface* screen_surface;
    int max;
    int dev_mod = argc != 3 ||argv[2][0] == '1';
    int hexa = argc==3 && (argv[2][0] == '1' ||argv[2][0] == '0' );
    // arg 3 == nothing -> devmod + !hexa
    // arg 3 == 1 -> devmod + hexa
    // arg 3 == 0 -> !devmod + hexa
    // arg 3 == other -> !devmod + !hexa

    // : Initialize the SDL
    init_sdl();

    // ----------------------Load_Image_and_resize------------------------------
    edge_surface = load_image(argv[1]);

    if(edge_surface->w > 1024 ||edge_surface->h > 1024)
        edge_surface = resize(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/original.png");

    // ----------------------Inverse-----------------------------------------
    edge_surface = inverse_color(edge_surface);

    // ----------------------Grayscale-----------------------------------------
    grayscale(edge_surface);

    make_thread(&out, edge_surface, "output/treatment/grayscale.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Increase_Contrast---------------------------------
    max = increase_contrast(edge_surface,10);
    make_thread(&out, edge_surface, "output/treatment/contrast.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Brightness----------------------------------------
    brightness(edge_surface,max);
    make_thread(&out, edge_surface, "output/treatment/brightness.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Reduce_Noise--------------------------------------
    edge_surface = reduce_noise(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/noise.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Blur----------------------------------------------
    edge_surface = blur(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/blur.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Adaptive_Threshold--------------------------------
    int noise_level = noise(edge_surface, edge_surface->w,edge_surface->h);
    adaptive_threshold(edge_surface, noise_level > 300 ? 0.5 : 0.15,32);
    make_thread(&out, edge_surface, "output/treatment/threshold.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Sobel---------------------------------------------
    edge_surface = Sobel(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/sobel.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);
    bin_surface = load_image("output/treatment/threshold.png");

    // ----------------------Hough_Transform_Rotate----------------------------
    edge_surface = hough_transform_rotate(edge_surface,&bin_surface,&out);
    // hough_transform_rotate save lines.png image with lines
    make_thread(&out, edge_surface, "output/treatment/rotate.png");

    if(dev_mod)
    {
        printf("process finish\n");
        screen_surface = display_image(edge_surface);
    }

    // ----------------------Blob_detection------------------------------------
    blob result = blobFromImage(edge_surface, 1000);

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Scale---------------------------------------------
    edge_surface = scale(edge_surface, result.min_x, result.min_y, result.max_x,result.max_y);
    bin_surface = scale(bin_surface, result.min_x, result.min_y, result.max_x,result.max_y);
    make_thread(&out, bin_surface, "output/treatment/scale.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    if(dev_mod)
        screen_surface = display_image(bin_surface);

    // ----------------------Correct_Perspective-------------------------------
    bin_surface = correct_perspective(edge_surface, bin_surface);
    bin_surface = bold(bin_surface, 1);
    make_thread(&out, bin_surface, "output/treatment/perspective.png");

    if(dev_mod)
    {
        screen_surface = display_image(bin_surface);
        printf("detection finish\n");
        wait_for_keypressed();
    }
    slicing(bin_surface,hexa);
    int *grid;
    int *boolean;
    int res;
    if(hexa)
    {
        grid = ocr_function("output/slot", 16);
        boolean = malloc(256*sizeof(int));
        for(int i = 0;i<256;i++)
        {
            if (grid[i] == 0)
                boolean[i] = 0;
            else
                boolean[i] = 1;
        }
        int **result = malloc(16*sizeof(int *));
        for (int i = 0; i<16; i++)
        {
            result[i] = malloc(16*sizeof(int));
            for(int j = 0; j<16; j++)
                result[i][j] = grid[i*16+j];
        }
        res = solve(result, 16);
    }
    else
    {
        grid = ocr_function("output/slot", 9);
        boolean = malloc(81*sizeof(int));
        for(int i = 0; i<81; i++)
        {
            if (grid[i] == 0)
                boolean[i] = 0;
            else
                boolean[i] = 1;
        }
        int **result = malloc(9*sizeof(int *));
        for (int i = 0; i<9; i++)
        {
            result[i] = malloc(9*sizeof(int));
            for(int j = 0; j<9; j++)
                result[i][j] = grid[i*9+j];
        }
        res = solve(result, 9);
    }
    res += 1;
    // ----------------------Free----------------------------------------------
    //SDL_FreeSurface(bin_surface);
    SDL_FreeSurface(edge_surface);
    if(dev_mod)
        SDL_FreeSurface(screen_surface);

    for(size_t i =0; i<out.th; i++)
        pthread_join(out.threads[i], NULL);

    free(out.threads);
    free(out.out);
    return 0;
}
