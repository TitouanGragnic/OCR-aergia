#include "../../include/training/training.h"

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

void initialize_outputs(double* outputs, size_t n, int acc)
{
    for(size_t i = 0; i < n; i++)
        outputs[i] = 0;
    outputs[acc] = 1;
}

int extract_res(double* outputs, int n)
{
    int i = 0;
	while(i < n && outputs[i] != 1)
		i++;
	if(i < 10)
		return i;
	printf("pepin\n");
	return 0;
}

Training load_training(char* path)
{
    size_t nb_tries = 30;
    size_t nb_characters = 10;
    size_t nb_set = nb_tries * nb_characters;
    size_t nb_inputs = 256;
    size_t nb_outputs = 10;

    Training training = { NULL, NULL, nb_set, nb_inputs, nb_outputs};
    training.inputs = malloc(sizeof(double) * training.nb_in * training.nb_set);
    training.outputs = malloc(sizeof(double) * training.nb_out * training.nb_set);

    SDL_Surface* img;
    char* file = malloc(sizeof(char) * 4096);
    for(size_t try = 1; try <= nb_tries; try++)
    {
        for(size_t i = 0; i < 10; i++)
        {
            sprintf(file, "%s%ld/%lu.png", path, i, try);
            img = load_image(file);
            printf("Load -> %s w:%i h:%i\n", file, img->w, img->h);
            size_t use = i + (try - 1) * nb_outputs;
            training_in(&training, use);
            img_to_matrix(img, training_in(&training, use));
            initialize_outputs(training_out(&training, use), nb_outputs, i);
            SDL_FreeSurface(img);
        }
    }
        free(file);
    return training;
}

