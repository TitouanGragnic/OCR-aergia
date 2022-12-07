#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "neural_network/neural_network.h"
#include "../grid_processing/include/utils/init.h"
#include "utils/img_to_matrix.h"
#include "utils/pixel_operations.h"

int main(int argc, char* argv[])
{
	//gérer les args + erreurs
/*
	size_t nb_layer = 4;
	size_t Layers[] = {256, 128, 64, 10};
	Network network = initialize_network(nb_layer, Layers);
    training_digits(100, 50, 100, network);
	free_network(network);
*/	
	int* final = ocr_function("../grid_processing/output/slot", 10);
	print_matrix(final, 9, 9);
	free(final);
	SDL_Surface* image = load_image("dataset/5/1.png");
	int res3 = compute_digits(image);
	printf("expected 5 - %d\n", res3);
	//create_dataset("dataset");
	SDL_FreeSurface(image);
	SDL_Surface* image2 = load_image("dataset/4/1.png");
	int res = compute_digits(image2);
	printf("%d - expected: 4\n", res);
	SDL_FreeSurface(image2);

	SDL_Surface* image3 = load_image("dataset/9/1.png");
	int res2 = compute_digits(image3);
	printf("%d - expected: 9\n", res2);
	SDL_FreeSurface(image3);

	SDL_Surface* image4 = load_image("dataset/0/1.png");
	int res4 = compute_digits(image4);
	printf("%d - expected: 0\n", res4);
	SDL_FreeSurface(image4);
    
	SDL_Surface* image5 = load_image("dataset/8/1.png");
	int res5 = compute_digits(image5);
	printf("%d - expected: 8\n", res5);
	SDL_FreeSurface(image5);

	SDL_Surface* image6 = load_image("dataset/7/1.png");
	int res6 = compute_digits(image6);
	printf("%d - expected: 7\n", res6);
	SDL_FreeSurface(image6);

	SDL_Surface* image7 = load_image("dataset/1/1.png");
	int res7 = compute_digits(image7);
	printf("%d - expected: 1\n", res7);
	SDL_FreeSurface(image7);

	SDL_Surface* image8 = load_image("dataset/2/1.png");
	int res8 = compute_digits(image8);
	printf("%d - expected: 2\n", res8);
	SDL_FreeSurface(image8);

	SDL_Surface* image9 = load_image("dataset/3/1.png");
	int res9 = compute_digits(image9);
	printf("%d - expected: 3\n", res9);
	SDL_FreeSurface(image9);

	SDL_Surface* image10 = load_image("dataset/6/1.png");
	int res10 = compute_digits(image10);
	printf("%d - expected: 6\n", res10);
	SDL_FreeSurface(image10);

	return 0;
}
