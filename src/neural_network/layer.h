#ifndef _LAYER_H_
#define _LAYER_H_

#include <stdio.h>
#include "training.h"

struct Layer
{
    size_t nb_neurons; //   => Number of neurons
    size_t w_per_neuron; // => Number of weights for each neurons

    double* weights; //           => Array of weights
    double* previous_dw; // => Array of weights before changement
    double* bias; //        => Array of bias
    double* errors; //         => Array of errors
    double* outputs; //         => Array of outputs
};

size_t get_w(struct Layer* layer, size_t i_n, size_t i_w);

void initialize_layer(struct Layer* layer,
 size_t nb_neurons, size_t w_per_neuron);

void free_layer(struct Layer* layer);

void calculate_output(struct Layer* layer, double* inputs);

void save_layer(struct Layer* layer, FILE* file, int first);

void load_layer(struct Layer* layer, FILE* file);
#endif
