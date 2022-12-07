#include "../../include/utils_neural/img_to_matrix.h"

void img_to_matrix(SDL_Surface* img, double* res)
{
    for(int i = 0; i < img->h; i++)
    {
        for(int j = 0; j < img->w; j++)
        {
            if (pixel_value(img, i, j) > 140) // == 255
                res[i*img->w + j] = 0;
            else
                res[i*img->w + j] = 1;
        }
    }
}

void print_matrix(int* array, size_t width, size_t height)
{
    for(size_t i = 0; i < width; i++)
        for(size_t j = 0; j < height; j++)
	{
	    if(j == width - 1)
	        printf("%d\n", array[j * width + i]);
	    else
	        printf("%d", array[j * width + i]);
	}
}

void save_matrix(FILE* fptr, double* matrix)
{
	for(size_t i = 0; i < EDGE_LENGTH; i++)
		for(size_t j = 0; j < EDGE_LENGTH; j++)
		{
			if(i == EDGE_LENGTH - 1 && j == EDGE_LENGTH - 1)
				fprintf(fptr, "%f\n", matrix[i * EDGE_LENGTH + j]);
			else
				fprintf(fptr, "%f", matrix[i * EDGE_LENGTH + j]);
		}
}

void create_dataset(char* path)
{
	DIR* pDir;
	struct dirent* pDirent;

	pDir = opendir (path);

    if (pDir == NULL)
		errx(1, "Cannot open directory '%s'\n", path);

    while ((pDirent = readdir(pDir)) != NULL)
	{
        if(!strcmp(pDirent->d_name, "..") || !strcmp(pDirent->d_name, "."))
			continue;

        char subpath[2 * (strlen(path) + strlen(pDirent->d_name) + 2)];

		strcpy(subpath, path);
		strcat(strcat(subpath, "/"), pDirent->d_name);
        strcat(subpath, "/");

		DIR* pSubDir = opendir(subpath);
        struct dirent *pSubDirent;

		char filename[strlen(pDirent->d_name) + 5];
		strcat(strcpy(filename, pDirent->d_name), ".txt");

		char filepath[strlen(subpath) + strlen(filename) + 2];
		strcpy(filepath, subpath);
		strcat(filepath, filename);

		FILE* fptr;
		remove(filepath);
		fptr = fopen(filepath, "w");

		if(fptr == NULL)
			errx(1, "An error occurred while opening the file.\n");

        while((pSubDirent = readdir(pSubDir)) != NULL)
		{
			if(!strcmp(pSubDirent->d_name, "..") ||
			!strcmp(pSubDirent->d_name, ".") || !strcmp(pSubDirent->d_name, filename))
				continue;
			char tmp[strlen(subpath) + 2 + strlen(pSubDirent->d_name)];
			strcpy(tmp, subpath);
			strcat(tmp, pSubDirent->d_name);
			SDL_Surface* image = load_image(tmp);
			double* array = malloc(sizeof(double) * 256);
			img_to_matrix(image, array);
			save_matrix(fptr, array);
			free(array);
			SDL_FreeSurface(image);
		}
		closedir(pSubDir);
		fclose(fptr);
    }
	closedir (pDir);
}
