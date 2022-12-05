#ifndef IMG_TO_MATRIX_H_
#define IMG_TO_MATRIX_H_

#include "pixel_operations.h"
#include "../../grid_processing/include/utils/init.h"
#include <stdio.h>
#include <dirent.h>

#define EDGE_LENGTH 16

void img_to_matrix(SDL_Surface* img, double* res);
void print_matrix(int* array, size_t width, size_t height);
void create_dataset(char* path);

#endif
