#include "neural_network.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

struct Network initialize_network(size_t nb_layers, size_t* nb_neurons_layer)
{
    /*
    create and initialize the neural network, by initializing each layer.
    return the network.
    */
    struct Network network;
    network.nb_layers = nb_layers;

    struct Layer* layer;
    layer = malloc(sizeof(struct Layer) * nb_layers);

    srand(time(NULL));

    initialize_layer(&layer[0], nb_neurons_layer[0], 0);
    for(size_t i = 1; i < nb_layers; i++)
        initialize_layer(&layer[i], nb_neurons_layer[i], nb_neurons_layer[i-1]);
    network.layers = layer;

    return network;
}

void free_network(struct Network network)
{
    /*
    free the network, free all the layers of the network.
    */
    for(size_t i = 0; i < network.nb_layers; i++)
        free_layer(network.layers + i);
    network.nb_layers = 0;
    free(network.layers);
}
void compute_network(struct Network network, double* inputs)
{
     /*
     computes the output of the network by calling calculate_output
     on every layer.
     */
     for(size_t i = 0; i < network.layers[0].nb_neurons; i++)
         network.layers[0].outputs[i] = inputs[i];
     for(size_t i = 1; i < network.nb_layers; i++)
         calculate_output(&network.layers[i], network.layers[i - 1].outputs);
}

void forward_prop(struct Network network, double* inputs, double* outputs)
{
    /*
    apply the forward propagation to the network.
    compute the outputs by calling compute_network,
    update the errors for each neuron of each layer.
    */
    compute_network(network, inputs);
    struct Layer* last = &network.layers[network.nb_layers - 1];
    for(size_t i = 0; i < last->nb_neurons; i++)
        last->errors[i] = sigmoid_prime(last->outputs[i]) * (outputs[i] - last->outputs[i]);
    
    for(size_t i = network.nb_layers - 2; i > 0; i--)
    {
        struct Layer* current = &network.layers[i];
        struct Layer* done = &network.layers[i + 1];
        for(size_t current_i = 0; current_i < current->nb_neurons; current_i++)
        {
            double sum = 0;
            for(size_t done_i = 0; done_i <done->nb_neurons; done_i++)
                sum += done->errors[done_i] * done->weights[get_w(done, done_i, current_i)];
            current->errors[current_i] = sigmoid_prime(current->outputs[current_i]) * sum;
        }
    }

}

void backward_prop(struct Network network)
{
    /*
    apply the backward propagation to the network,
    update weights and bias of each layer calculating dw/db.
    */
    double l_rate = 0.4;

    for(size_t i = 1; i < network.nb_layers; i++)
    {
        struct Layer* current = &network.layers[i];
        struct Layer* done = &network.layers[i - 1];
        for(size_t current_i = 0; current_i < current->nb_neurons; current_i++)
        {
            current->bias[current_i] += l_rate * current->errors[current_i];
            for(size_t done_i = 0; done_i < done->nb_neurons; done_i++)
            {
                size_t j = get_w(current, current_i, done_i);
                double deltaw = l_rate * current->errors[current_i] * done->outputs[done_i];
                current->weights[j] += deltaw + 0.1 * current->previous_dw[j];
                current->previous_dw[j] = deltaw;
            }
        }
    }
}

double* output_network(struct Network network)
{
    /*
    round up or down the outputs of the last layer to get final results.
    */
    struct Layer* layer = &network.layers[network.nb_layers - 1];
    for(size_t i = 0; i < layer->nb_neurons; i++)
        layer->outputs[i] = (layer->outputs[i] > 0.5) ? 1 : 0;
    return layer->outputs;
}

void train_network(struct Network network, struct Training training, int print, char string[])
{
    /*
    train the network for training.nb_set iterations,
    by calling forward_prop and then backward_prop,
    print when print boolean is true.
    */
    size_t i = 0;
    for(; i < training.nb_set; i++)
    {
        forward_prop(network, training_in(&training, i), training_out(&training, i));
        backward_prop(network);
        if(print)
            print_training(network, training, i, string);
    }
}

void print_training(struct Network network,struct Training training, size_t i, char* string)
{
    /*
    print the training.
    */
    double* arr = training_in(&training, i);
    for(size_t j = 0; j < training.nb_in - 1; j++)
        printf("%.1f %s ", arr[j], string);
    printf("%.1f ", arr[training.nb_in - 1]);
    printf("= ");
    for(size_t j = 0; j < training.nb_out; j++)
        printf("%f ", network.layers[network.nb_layers - 1].outputs[j]);
    printf("(expected = ");
    arr = training_out(&training, i);
    for(size_t j = 0; j < training.nb_out; j++)
        printf("%.1f", arr[j]);
    printf(")\n");
}

