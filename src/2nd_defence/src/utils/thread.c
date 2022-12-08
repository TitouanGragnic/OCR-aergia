#include "../../include/utils/thread.h"

static void* saveThead(void* input_i)
{
    img_file* input = input_i;
    SDL_SaveBMP(input->img, input->file);
    SDL_FreeSurface(input->img);
    pthread_exit(NULL);
}

void make_thread(img_thread* data, SDL_Surface* img, char* path)
{
    img_file* out = data->out;
    size_t th = data->th;
    pthread_t* threads = data->threads;
    out[th].img = copy_surface(img);
    out[th].file = path;
    pthread_create(&threads[th], NULL, saveThead, &out[th]);
    data->th++;
}
