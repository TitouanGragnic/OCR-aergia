#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
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

#include "include/slot_processing/slicing.h"

struct carth{
    int x;
    int y;
};

struct img_file{
    SDL_Surface* img;
    char* file;
};

static void* saveThead(void* input_i)
{
    struct img_file* input = input_i;
    SDL_SaveBMP(input->img, input->file);
    SDL_FreeSurface(input->img);
    pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
    // ----------------------Assert---------------------------------------------
    if(argc > 3 || argc == 1)
        errx(1, "Type wanted is : ./out file.png dev_mod\n");

    size_t NUM_THREADS = 12*2;
    size_t th = 0;
    pthread_t threads[NUM_THREADS];

    SDL_Surface* edge_surface;
    SDL_Surface* bin_surface;
    SDL_Surface* screen_surface;
    int max;
    int dev_mod = argc !=3;

    // : Initialize the SDL
    init_sdl();

    // ----------------------Load_Image_and_resize------------------------------
    edge_surface = load_image(argv[1]);

    if(edge_surface->w > 1024 ||edge_surface->h > 1024)
        edge_surface = resize(edge_surface);

    // ----------------------Grayscale-----------------------------------------
    grayscale(edge_surface);
    struct img_file out;
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/grayscale.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;
    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Increase_Contrast---------------------------------
    max = increase_contrast(edge_surface,10);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/contrast.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Brightness----------------------------------------
    brightness(edge_surface,max);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/brightness.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Reduce_Noise--------------------------------------
    edge_surface = reduce_noise(edge_surface);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/noise.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Blur----------------------------------------------
    edge_surface = blur(edge_surface);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/blur.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Adaptive_Threshold--------------------------------
    int noise_level = noise(edge_surface, edge_surface->w,edge_surface->h);
    adaptive_threshold(edge_surface, noise_level > 300 ? 0.5 : 0.15,32);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/threshold.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Sobel---------------------------------------------
    edge_surface = Sobel(edge_surface, 1);
    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/sobel.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);
    bin_surface = load_image("output//treatment/threshold.png");

    // ----------------------Hough_Transform_Rotate----------------------------
    edge_surface = hough_transform_rotate(edge_surface,&bin_surface);
    // hough_transform_rotate save lines.png image with lines

    out.img = copy_surface(edge_surface);
    out.file = "output/treatment/rotate.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

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

    out.img = copy_surface(bin_surface);
    out.file = "output/treatment/scale.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    if(dev_mod)
        screen_surface = display_image(bin_surface);

    // ----------------------Correct_Perspective-------------------------------
    bin_surface = correct_perspective(edge_surface, bin_surface);

    out.img = copy_surface(bin_surface);
    out.file = "output/treatment/perspective.png";
    pthread_create(&threads[th], NULL, saveThead, &out);
    th++;
    if(dev_mod)
    {
        screen_surface = display_image(bin_surface);
        printf("detection finish\n");
        wait_for_keypressed();
    }

    slicing(bin_surface);

    // ----------------------Free----------------------------------------------
    //SDL_FreeSurface(bin_surface);
    SDL_FreeSurface(edge_surface);
    if(dev_mod)
        SDL_FreeSurface(screen_surface);

    for(size_t i =0; i<th; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
