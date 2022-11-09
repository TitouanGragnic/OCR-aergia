#ifndef _TRAINING_H_
#define _TRAINING_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Training
{
    double* inputs; //      => Array of inputs data

    double* outputs; //     => Array of outputs data

    size_t nb_set; //   => Number of set data

    size_t nb_in; //    => Number of input data
    size_t nb_out; //   => Number of output data
};

double* training_in(struct Training* training, size_t i);

double* training_out(struct Training* training, size_t i);

double sigmoid(double x);

double sigmoid_prime(double x);

void free_training(struct Training training);

#endif
