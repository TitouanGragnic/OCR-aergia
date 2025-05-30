#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "neural_network/neural_network.h"

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

    Training training = { NULL, NULL, 4, 2, 1 };

    double inputs[] = { 0, 0, 0, 1, 1, 0, 1, 1};

    double outputs[] = { 0, 1, 1, 0};

    training.inputs = inputs;
    training.outputs = outputs;

    Network network = initialize_network(nb_layer, Layers);

    /*
    training of the network.
    */
    for(size_t i = 0; i <= nb_trains; i++)
    {
        if(i % 100 == 0) 
            printf("iteration = %lu\n", i);

        train_network(network, training, (i % 100 == 0), string);
    }

    save_network(network, "logs/xor.txt");
    printf("Network successfully saved.\n");
    Network network2 = load_network("logs/xor.txt");
    printf("Network successfully loaded.\n");

    /*
    part that manages the user input and sends back the result 
    found by the network.
    */
    char user_input[5];
    printf("\n");
    printf("Enter 2 integers which are part of the train set separated\n");
    printf("by a space and I will compute the result.\n");
    printf("\n");
    printf("Write \"exit\" to leave.\n");
    scanf("%4[^\n]%*c", user_input);

    int number = 0;
    int space = 0;

    while(strcmp(user_input, "exit"))
    {
        double values[] = {0, 0};

        size_t i = 0;
        size_t j = 0;

        while(user_input[i])
        {
            if(user_input[i] == '0' || user_input[i] == '1')
            {
                if(number && !space)
                    errx(1, "Wrong inputs, bye.\n");
                if(j < 2)
                {
                    values[j] = user_input[i] - '0';
                    j += 1;
                }
                number = 1;
                space = 0;
            }
            else if(user_input[i] != ' ' && user_input[i] != -64)
                errx(1, "Wrong inputs, bye.\n");
            else
                space = 1;
            i += 1;
        }
        compute_network(network2, values);
        double result = network2.layers[network2.nb_layers - 1].outputs[0];
        result = result > 0.5f ? 1 : 0;

        printf("%.1f XOR %.1f = %f <=> %.1f\n", values[0], values[1], 
            network2.layers[network2.nb_layers - 1].outputs[0], result);
        number = 0;
        space = 0;
        scanf("%4[^\n]%*c", user_input);
    }

    free_network(network);
    free_network(network2);
    return 0;
}
