#include "layer.h"


double randform(void)
{
    /*
    return a pseudo random number.
    */
    return -1. + 2. * ((double)rand() / RAND_MAX);
}

size_t get_w(Layer* layer, size_t i_n, size_t i_w)
{
    /*
    return the weight of index i_w.
    */
    return i_w + i_n * layer->w_per_neuron;
}

void initialize_layer(Layer* layer, size_t nb_neurons,
 size_t w_per_neuron)
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

void free_layer(Layer* layer)
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

void calculate_output(Layer* layer, double* inputs)
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

void save_layer(Layer* layer, FILE* file, int first)
{
    /*
    write all the data of the layer if the stream file
    given in parameter.
    */
    if(first)
    {
    	fprintf(file, "%ld\n", layer->nb_neurons);
	    fprintf(file, "%ld\n", layer->w_per_neuron);
    }
    else
    {
    	fprintf(file, "%ld\n", layer->nb_neurons);
	    fprintf(file, "%ld\n", layer->w_per_neuron);

	    for(size_t i = 0; i < layer->nb_neurons; i++)
	    {
	        fprintf(file, "%lf\n", layer->bias[i]);
	        for(size_t j = 0; j < layer->w_per_neuron; j++)
	        {
	            fprintf(file, "%lf\n", layer->weights[get_w(layer, i, j)]);
	        }
	    }
    }
}

void load_layer(Layer* layer, FILE* file)
{
    /*
    read all the data of a layer from a stream file
    given in parameter, and put them into the layer
    struct.
    */
    fscanf(file, "%lu", &layer->nb_neurons);
    fscanf(file, "%lu", &layer->w_per_neuron);

    if(layer->w_per_neuron)
    {
        layer->weights = malloc(sizeof(double) * layer->nb_neurons
         * layer->w_per_neuron);
        layer->previous_dw = calloc(sizeof(double), 
        layer->nb_neurons * layer->w_per_neuron);
        layer->bias = malloc(sizeof(double) * layer->nb_neurons);
        layer->errors = calloc(sizeof(double), layer->nb_neurons);
        for(size_t i = 0; i < layer->nb_neurons; i++)
        {
            fscanf(file, "%lf", &layer->bias[i]);
            for(size_t j = 0; j < layer->w_per_neuron; j++)
                fscanf(file, "%lf", &layer->weights[get_w(layer, i, j)]);
        }
    }
    else
    {
        layer->weights = NULL;
        layer->previous_dw = NULL;
        layer->bias = NULL;
        layer->errors = NULL;
    }
    layer->outputs = malloc(sizeof(double) * layer->nb_neurons);
}
