#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "neural_network.h"

int main(int argc, char* argv[])
{
    /*
    part that manages the arguments.
    */
    if(argc > 2)
        errx(1, "usage: ./xor <number of training>\n            ./xor");
    size_t nb_trains = 5000;
    if(argc == 2)
    {
        unsigned long param = strtoul(argv[1], NULL, 10);
        if(param == 0)
            errx(1,"usage: ./xor <number of training>\n            ./xor");
        nb_trains = param * 1000;
    }

    /*
    creation of the neural network.
    */
    char string[] = "XOR";

    size_t nb_layer = 3;

    size_t Layers[] = { 2, 2, 1 };

    struct Training training = { NULL, NULL, 4, 2, 1 };

    double inputs[] = { 0, 0, 0, 1, 1, 0, 1, 1};

    double outputs[] = { 0, 1, 1, 0};

    training.inputs = inputs;
    training.outputs = outputs;

    struct Network network = initialize_network(nb_layer, Layers);

    /*
    training of the network.
    */
    for(size_t i = 0; i <= nb_trains; i++)
    {
        if(i % 100 == 0) 
            printf("iteration = %lu\n", i);

        train_network(network, training, (i % 100 == 0), string);
    }
    
    /*
    part that manages the user input and sends back the result found by the network.
    */
    char user_input[5];
    scanf("%4[^\n]%*c", user_input);

    while(strcmp(user_input, "exit"))
    {
        double values[] = {0, 0};

        size_t i = 0;
        size_t j = 0;

        while(user_input[i])
        {
            if(j < 2 && (user_input[i] == '0' || user_input[i] == '1'))
            {
                values[j] = user_input[i] - '0';
                j += 1;
            }
            else if(user_input[i] != ' ' && user_input[i] != -64)
                errx(1, "this is not the way.\n");
            i += 1;
        }
        compute_network(network, values);
        double result = network.layers[network.nb_layers - 1].outputs[0];
        result = result > 0.5f ? 1 : 0;

        printf("%.1f XOR %.1f = %f <=> %.1f\n", values[0], values[1], 
            network.layers[network.nb_layers - 1].outputs[0], result);

        scanf("%4[^\n]%*c", user_input);
    }

    free_network(network);
    return 0;
}
