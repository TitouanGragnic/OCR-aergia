#ifndef THREAD_H_
#define THREAD_H_

#include "init.h"
#include "draw.h"
#include <pthread.h>

typedef struct{
    SDL_Surface* img;
    char* file;
} img_file;

typedef struct{
    size_t th;
    pthread_t *threads;
    img_file* out;
} img_thread;

//static void* save_thread(void* input_i);
void make_thread(img_thread* data, SDL_Surface* img, char* path);

#endif
