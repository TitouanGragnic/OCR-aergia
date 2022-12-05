#include "../../include/neural_network/neural_network.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Network initialize_network(size_t nb_layers, size_t* nb_neurons_layer)
{
    /*
    create and initialize the neural network, by initializing each layer.
    return the network.
    */
    Network network;
    network.nb_layers = nb_layers;

    Layer* layer;
    layer = malloc(sizeof(Layer) * nb_layers);

    srand(time(NULL));

    initialize_layer(&layer[0], nb_neurons_layer[0], 0);
    for(size_t i = 1; i < nb_layers; i++)
        initialize_layer(&layer[i], nb_neurons_layer[i],
         nb_neurons_layer[i-1]);
    network.layers = layer;

    return network;
}

void free_network(Network network)
{
    /*
    free the network, free all the layers of the network.
    */
    for(size_t i = 0; i < network.nb_layers; i++)
        free_layer(network.layers + i);
    network.nb_layers = 0;
    free(network.layers);
}

void compute_network(Network network, double* inputs)
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

double error_network(Network network)
{
	double error = 0;
	Layer* last_layer = &network.layers[network.nb_layers - 1];
	for(size_t i = 0; i < last_layer->nb_neurons; i++)
		error += pow(last_layer->errors[i], 2);
	return error * 0.5;
}

void forward_prop(Network network, double* inputs, double* outputs, double* error)
{
    /*
    apply the forward propagation to the network.
    compute the outputs by calling compute_network,
    update the errors for each neuron of each layer.
    */
    compute_network(network, inputs);
	*error += error_network(network);
    Layer* last = &network.layers[network.nb_layers - 1];
    for(size_t i = 0; i < last->nb_neurons; i++)
        last->errors[i] =
         sigmoid_prime(last->outputs[i]) * (outputs[i] - last->outputs[i]);

    for(size_t i = network.nb_layers - 2; i > 0; i--)
    {
        Layer* current = &network.layers[i];
        Layer* done = &network.layers[i + 1];
        for(size_t current_i = 0; current_i < current->nb_neurons; current_i++)
        {
            double sum = 0;
            for(size_t done_i = 0; done_i <done->nb_neurons; done_i++)
                sum += done->errors[done_i] *
                 done->weights[get_w(done, done_i, current_i)];
            current->errors[current_i] =
            sigmoid_prime(current->outputs[current_i]) * sum;
        }
    }

}

void backward_prop(Network network, char string[])
{
    /*
    apply the backward propagation to the network,
    update weights and bias of each layer calculating dw/db.
    */
    double l_rate;
	if(!strcmp(string, "XOR"))
		l_rate = 0.4;
	else
		l_rate = 0.01;

    for(size_t i = 1; i < network.nb_layers; i++)
    {
        Layer* current = &network.layers[i];
        Layer* done = &network.layers[i - 1];
        for(size_t current_i = 0; current_i < current->nb_neurons; current_i++)
        {
            current->bias[current_i] += l_rate * current->errors[current_i];
            for(size_t done_i = 0; done_i < done->nb_neurons; done_i++)
            {
                size_t j = get_w(current, current_i, done_i);
                double deltaw =
                l_rate * current->errors[current_i] * done->outputs[done_i];
                current->weights[j] += deltaw + 0.1 * current->previous_dw[j];
                current->previous_dw[j] = deltaw;
            }
        }
    }
}

double* output_network(Network network)
{
    /*
    round up or down the outputs of the last layer to get final results.
    */
    Layer* layer = &network.layers[network.nb_layers - 1];
    for(size_t i = 0; i < layer->nb_neurons; i++)
        layer->outputs[i] = (layer->outputs[i] > 0.5) ? 1 : 0;
    return layer->outputs;
}

void train_network(Network network, Training training,
 int print, char string[])
{
    /*
    train the network for training.nb_set iterations,
    by calling forward_prop and then backward_prop,
    print when print boolean is true.
    */

	double error = 0;
    for(size_t i = 0; i < training.nb_set; i++)
    {
        forward_prop(network, training_in(&training, i),
         training_out(&training, i), &error);
        backward_prop(network, string);
        if(print)
            print_training(network, training, i, error, string);
    }
}

void print_training(Network network, Training training,
 size_t i, double error, char* string)
{
    /*
    print the training.
    */
	if(!strcmp(string, "XOR"))
	{
    	printf("\033[0;37m(expected = ");
    	double* array = training_out(&training, i);
    	for(size_t j = 0; j < training.nb_out; j++)
        	printf("\033[0;31m%.1f", array[j]);
    	printf(" \033[0;37m) ");
    	for(size_t j = 0; j < training.nb_out; j++)
        	printf(" \033[0;32m%f ",
            	network.layers[network.nb_layers - 1].outputs[j]);
    	printf("\033[0;37m<- ");
    	array = training_in(&training, i);
    	for(size_t j = 0; j < training.nb_in - 1; j++)
        	printf("%.1f %s ", array[j], string);
    	printf("%.1f ", array[training.nb_in - 1]);
    	printf("\n");
	}

	if(!strcmp(string, "DIGITS"))
		printf("error: %.15f\n", error);
}

void training_digits(size_t n, int print, int save, Network network)
{
    Training training = load_training("dataset/");
    for(size_t i = 0; i <= n; i++)
    {
        train_network(network, training, (i % print) == 0, "DIGITS");
        if((i % print) == 0)
            printf("EPOCH = %lu\n", i);
        if((i % save) == 0)
            save_network(network, "logs/digits.txt");
    }
    free_training(training);
}

int compute_digits(SDL_Surface* image)
{
    Network network = load_network("logs/digits.txt");
    double inputs[256];
    img_to_matrix(image, inputs);
    compute_network(network, inputs);
    int res = extract_res(output_network(network), 10);
    free_network(network);
    return res;
}

int* final_function(char* path, int nb_output)
{
        int* res = malloc(sizeof(int) * nb_output * nb_output);
        char filepath[4096];
        int tmp = nb_output;
        for(int i = 0; i < nb_output - 1; i++)
        {
                if(i == nb_output - 2)
                        tmp = 1;
                for(int j = 0; j < tmp; j++)
                {
                        sprintf(filepath, "%s/slot%d%d.png", path, i, j);
                        SDL_Surface* image = load_image(filepath);
                        res[i * nb_output + j] = compute_digits(image);
                        SDL_FreeSurface(image);
                }
        }
        return res;
}

void save_network(Network network, const char* path)
{
    /*
    save the network to the file specified in parameter;
    for each layer, call the function save_layer.
    */
    FILE* fptr;
    fptr = fopen(path, "w");
    if(fptr == NULL)
        errx(1, "Path is invalid, we were unable to find the file.");
    else
    {
        fprintf(fptr, "%ld\n", network.nb_layers);
        for(size_t i = 0; i < network.nb_layers; i++)
	        save_layer(&network.layers[i], fptr, i == 0);
    }
    fclose(fptr);
}

Network load_network(const char* path)
{
    /*
    load the network from a file specified in parameter;
    return a struct Network, the network.
    */
    FILE* file = fopen(path, "r");

    if(!file)
        errx(1, "Path is invalid, we were unable to find the file.");

    Network network;
    fscanf(file, "%lu", &network.nb_layers);
    network.layers = malloc(sizeof(Layer) * network.nb_layers);

    for(size_t i = 0; i < network.nb_layers; i++)
    {
        load_layer(&network.layers[i], file);
    }
    fclose(file);
    return network;
}
