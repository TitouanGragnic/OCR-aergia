#include "../../include/color_treatment/noise.h"

void sort(int* tab,int len)
{
    /*
      quick sort
    */
    if(len < 2)
        return;

    int pivot = tab[len / 2];

    int i, j;
    for (i = 0, j = len - 1; ; i++, j--)
    {
        while(tab[i] < pivot)
            i++;
        while(tab[j] > pivot)
            j--;

        if(i >= j)
            break;

        int temp = tab[i];
        tab[i] = tab[j];
        tab[j] = temp;
    }

    sort(tab, i);
    sort(tab + i, len - i);
}

SDL_Surface *reduce_noise(SDL_Surface *img)
{
    /*
      apply filter to reduce noise of img
    */
    SDL_Surface *oi = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
    int *matr = malloc(9*sizeof(int));
    int index = 0;

    for (int i = 0; i < img->w; i++) {
	for (int j = 0; j < img->h; j++) {
            index = 0;
            for(int ki=-1; ki <= 1; ki++) {
                for(int kj=-1; kj <= 1; kj++) {
                    if (i+ki >= 0 && i+ki < img->w && j+kj >= 0 && j+kj < img->h)
                        matr[index] = pixel_value(img, i + ki, j + kj);
                    else
                        matr[index] = 0;
                    index += 1;
                }
            }
            sort(matr,9);
            put_pixel_value(oi, i, j, matr[4]);
	}
    }
    free(matr);
    SDL_FreeSurface(img);
    return oi;
}

int noise(SDL_Surface* img, int w, int h)
{
    /*
      return noise level on image done
    */
    double sum = 0;
    int count = 0;
    double val = 0;

    for (int i = 0; i<w; i++)
    {
        for (int j = 0; j<h; j++)
        {
            sum=0;
            for(int ki = -1; ki <= 1; ki++)
                for(int kj = -1; kj <= 1; kj++)
                    if(i+ki >= 0 && i+ki < img->w && j+kj >= 0 && j+kj < img->h)
                        sum += pixel_value(img, i + ki, j + kj);
            sum = sum/9.0;
            double color = pixel_value(img, i, j);
            val = 1 - color / sum;
            if(val < 0)
                val *= -1;
            if(val > 0.5)
                count += 1;
        }
    }
    return count;
}
