#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "layer.h"
#include "training.h"

struct Network
{
    size_t nb_layers; //    => Number of layers

    struct Layer* layers; //=> Array of layers
};

struct Network initialize_network(size_t nb_layers, size_t* nb_neurons_layer);

void free_network(struct Network network);

void train_network(struct Network network, struct Training training,
 int print, char string[]);

void compute_network(struct Network network, double* inputs);

void print_training(struct Network network, struct Training training,
 size_t i, char* string);

void save_network(struct Network network, const char* path);

struct Network load_network(const char* path);
#endif
