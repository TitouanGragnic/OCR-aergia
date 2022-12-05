#include "../../include/slot_processing/slicing.h"

#include <pthread.h>

typedef struct{
    SDL_Surface* main_surface;
    int i;
    int j;
    int number;
    int width;
}slot_thread;

void* thread_slot(void* data_i)
{
    slot_thread* data = data_i;
    SDL_Surface* slot_surface;
    SDL_Surface* copy;
    SDL_Surface* result_surface;
    SDL_Surface* main_surface = data->main_surface;

    char file_name[] = "output/slot/slot000.png";

    int w = main_surface->w;
    int h = main_surface->h;
    int newW = w/data->width;//here to change the number of grid per row
    int newH = h/data->width;//per lines

    slot_surface = SDL_CreateRGBSurface(0, newW, newH,32,0,0,0,0);
    copy = SDL_CreateRGBSurface(0, newW, newH,32,0,0,0,0);
    for(int x = 0;x<newW;x++)
    {
        for(int y = 0;y<newH;y++)
        {
            int val = pixel_value(main_surface,data->i*newW+x,data->j*newH+y);
            put_pixel_value(slot_surface,x,y,val);
            put_pixel_value(copy,x,y,val);
        }
    }
    result_surface = resize_slot(copy);
    slot_surface = resize_slot(slot_surface);
    for(int x = 0;x<16;x++)
    {
        for(int y = 0;y<16; y++)
        {
            int val = pixel_value(result_surface,x,y);
            if (val<128)
                put_pixel_value(result_surface,x,y,0);
            else
                put_pixel_value(result_surface,x,y,255);
        }
    }
    for (int x = 0;x<16;x++)
    {
        for (int y = 0; y<3; y++)
            put_pixel_value(result_surface,x,y,255);
        for (int y = 13;y<16;y++)
            put_pixel(result_surface,x,y,255);
    }
    for (int y = 2;y<14;y++)
    {
        for (int x = 0;x<3; x++)
            put_pixel(result_surface,x,y,255);
        for (int x = 13;x<16;x++)
            put_pixel_value(result_surface,x,y,255);
    }
    slot_surface = blobSlot(result_surface, 0, slot_surface);

    file_name[16] = '0'+data->number/100;
    data->number = data->number%100;
    file_name[17] = '0'+data->number/10;
    file_name[18] = '0'+data->number%10;
    SDL_SaveBMP(slot_surface, file_name);
    SDL_FreeSurface(slot_surface);
    pthread_exit(NULL);
}


void slicing(SDL_Surface* main_surface, int hexa)
{
    /*
      slices the grid into 81 slots and saves them in output
    */
    int number = 0;
    size_t width;
    slot_thread* data;
    if(hexa)
        width = 16;
    else
        width = 9;
    data = malloc(sizeof(slot_thread)*width*width);
    pthread_t* threads = malloc(sizeof(pthread_t)*width*width);

    for (size_t i = 0;i<width;i++)
    {
        for (size_t j = 0;j<width;j++)
	{
            data[number].main_surface = main_surface;
            data[number].i = i;
            data[number].j = j;
            data[number].number = number;
            data[number].width = width;
            pthread_create(&threads[number], NULL, thread_slot, &data[number]);
            number++;
	}
    }

    for(size_t i =0; i<width*width; i++)
        pthread_join(threads[i], NULL);
    free(data);
    free(threads);
    SDL_FreeSurface(main_surface);
}
