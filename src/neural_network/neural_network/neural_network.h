#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../layer/layer.h"
#include "../training/training.h"

typedef struct
{
    size_t nb_layers; //    => Number of layers

    Layer* layers; //=> Array of layers
} Network;

Network initialize_network(size_t nb_layers, size_t* nb_neurons_layer);

void free_network(Network network);

void train_network(Network network, Training training,
 int print, char string[]);

void compute_network(Network network, double* inputs);

double* output_network(Network network);

void print_training(Network network, Training training,
 size_t i, double error, char* string);

void save_network(Network network, const char* path);

Network load_network(const char* path);

void training_digits(size_t n, int print, int save, Network network);

int compute_digits(SDL_Surface* image);

int* ocr_function(char* path, int nb_output);

#endif
