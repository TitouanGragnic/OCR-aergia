#include "layer.h"


double randform(void)
{
    /*
    return a pseudo random number.
    */
    return -1. + 2. * ((double)rand() / RAND_MAX);
}

size_t get_w(struct Layer* layer, size_t i_n, size_t i_w)
{
    /*
    return the weight of index i_w.
    */
    return i_w + i_n * layer->w_per_neuron;
}

void initialize_layer(struct Layer* layer, size_t nb_neurons, size_t w_per_neuron)
{
    /*
    initialize a layer.
    */
    layer->nb_neurons = nb_neurons;
    layer->w_per_neuron = w_per_neuron;

    if(w_per_neuron)
    {
        layer->weights = malloc(sizeof(double) * nb_neurons * w_per_neuron);
        layer->previous_dw = calloc(sizeof(double), nb_neurons * w_per_neuron);
        layer->bias = malloc(sizeof(double) * nb_neurons);
        layer->errors = calloc(sizeof(double), nb_neurons);

        for(size_t i = 0; i < nb_neurons; i++)
        {
            layer->bias[i] = randform();
            for(size_t j = 0; j < w_per_neuron; j++)
                layer->weights[get_w(layer, i ,j)] = randform();
        }
    }
    else
    {
        layer->weights = NULL;
        layer->previous_dw = NULL;
        layer->bias = NULL;
        layer->errors = NULL;
    }
    layer->outputs = malloc(sizeof(double) * nb_neurons);
}

void free_layer(struct Layer* layer)
{
    /*
    free all the data of the layer.
    */
    free(layer->weights);
    free(layer->previous_dw);
    free(layer->bias);
    free(layer->errors);
    free(layer->outputs);
}

void calculate_output(struct Layer* layer, double* inputs)
{
    /*
    compute the ouputs of a layer depending on the inputs,
    by applying the sigmoid function.
    */
    for(size_t i = 0; i < layer->nb_neurons; i++)
    {
        double si = 0;
        for(size_t j = 0; j < layer->w_per_neuron; j++)
            si += layer->weights[get_w(layer, i, j)] * inputs[j];
        layer->outputs[i] = sigmoid(si + layer->bias[i]);
    }
}
