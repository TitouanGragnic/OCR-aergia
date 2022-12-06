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
    SDL_Surface* main_surface = data->main_surface;

    char file_name[] = "output/slot/slot000.png";

    int w = main_surface->w;
    int h = main_surface->h;
    int newW = w/data->width;//here to change the number of grid per row
    int newH = h/data->width;//per lines

    slot_surface = SDL_CreateRGBSurface(0, newW, newH,32,0,0,0,0);
    for(int x = 0;x<newW;x++)
        for(int y = 0;y<newH;y++)
        {
            int val = pixel_value(main_surface,data->i*newW+x,data->j*newH+y);
            put_pixel_value(slot_surface,x,y,val);
        }
    int s = 15*newH/150;

    slot_surface = resize_slot2(slot_surface,newW/2+s,s,newH-s);


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
