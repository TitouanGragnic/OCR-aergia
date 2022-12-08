#include "training.h"

double* training_in(Training* training, size_t i)
{
    /*
    return the input of index i.
    */
    return &training->inputs[i * training->nb_in];
}

double* training_out(Training* training, size_t i)
{
    /*
    return the output of index i.
    */
    return &training->outputs[i * training->nb_out];
}

double sigmoid(double x)
{
    /*
    sigmoid function.
    */
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_prime(double x)
{
    /*
    derivative function of sigmoid.
    */
    return (1.0 - x) * x;
}

void free_training(Training training)
{
    /*
    free all training data.
    */
    free(training.inputs);
    free(training.outputs);
    training.nb_set = 0;
    training.nb_in = 0;
    training.nb_out = 0;
}
