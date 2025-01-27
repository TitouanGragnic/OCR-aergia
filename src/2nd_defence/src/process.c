#include "../include/process.h"


void process(char* path, int hexa, int dev_mod)
{
    // ----------------------Assert---------------------------------------------
    size_t NUM_THREADS = 13*2;

    img_thread out;
    out.th = 0;
    out.threads = malloc(sizeof(pthread_t)*NUM_THREADS);
    out.out = malloc(sizeof(img_file)*NUM_THREADS);

    SDL_Surface* edge_surface;
    SDL_Surface* bin_surface;
    SDL_Surface* screen_surface = NULL;
    int max;
    // arg 3 == nothing -> devmod + !hexa
    // arg 3 == 1 -> devmod + hexa
    // arg 3 == 0 -> !devmod + hexa
    // arg 3 == other -> !devmod + !hexa

    // : Initialize the SDL
    init_sdl();

    // ----------------------Load_Image_and_resize------------------------------
    edge_surface = load_image(path);

    if(edge_surface->w > 1024 ||edge_surface->h > 1024)
        edge_surface = resize(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/original.png");

    // ----------------------Inverse-----------------------------------------
    edge_surface = inverse_color(edge_surface);

    // ----------------------Grayscale-----------------------------------------
    grayscale(edge_surface);

    make_thread(&out, edge_surface, "output/treatment/grayscale.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Increase_Contrast---------------------------------
    max = increase_contrast(edge_surface,10);
    make_thread(&out, edge_surface, "output/treatment/contrast.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Brightness----------------------------------------
    brightness(edge_surface,max);
    make_thread(&out, edge_surface, "output/treatment/brightness.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Reduce_Noise--------------------------------------
    edge_surface = reduce_noise(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/noise.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Blur----------------------------------------------
    edge_surface = blur(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/blur.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Adaptive_Threshold--------------------------------
    int noise_level = noise(edge_surface, edge_surface->w,edge_surface->h);
    adaptive_threshold(edge_surface, noise_level > 300 ? 0.5 : 0.15,32);
    int th_threshold = out.th;
    make_thread(&out, edge_surface, "output/treatment/threshold.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Sobel---------------------------------------------
    edge_surface = Sobel(edge_surface);
    make_thread(&out, edge_surface, "output/treatment/sobel.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    pthread_join(out.threads[th_threshold], NULL);
    bin_surface = load_image("output/treatment/threshold.png");

    // ----------------------Hough_Transform_Rotate----------------------------
    edge_surface = hough_transform_rotate(edge_surface,&bin_surface,&out);
    // hough_transform_rotate save lines.png image with lines
    make_thread(&out, edge_surface, "output/treatment/rotate.png");

    if(dev_mod)
    {
        printf("process finish\n");
        screen_surface = display_image(edge_surface);
    }

    // ----------------------Blob_detection------------------------------------
    blob result_blob = blobFromImage(edge_surface, 1000);

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    // ----------------------Scale---------------------------------------------
    edge_surface = scale(edge_surface, result_blob.min_x, result_blob.min_y, result_blob.max_x,result_blob.max_y);
    bin_surface = scale(bin_surface, result_blob.min_x, result_blob.min_y, result_blob.max_x,result_blob.max_y);
    make_thread(&out, bin_surface, "output/treatment/scale.png");

    if(dev_mod)
        screen_surface = display_image(edge_surface);

    if(dev_mod)
        screen_surface = display_image(bin_surface);

    // ----------------------Correct_Perspective-------------------------------
    bin_surface = correct_perspective(edge_surface, bin_surface);
    bin_surface = bold(bin_surface, 1);
    make_thread(&out, bin_surface, "output/treatment/perspective.png");

    if(dev_mod)
    {
        screen_surface = display_image(bin_surface);
        printf("detection finish\n");
        wait_for_keypressed();
    }
    slicing(bin_surface,hexa);
	int *grid;
    int *boolean;
    int res;
    int width = hexa ? 16 : 9;
/*
	size_t nb_layer = 5;
	size_t Layers[] = {784, 392, 196, 98, 10};
	Network network = initialize_network(nb_layer, Layers);
	training_digits(30, 10, 10, network);
	free_network(network);
*/
    grid = ocr_function("output/slot", width + 1);
    if(dev_mod)
        print_matrix(grid, width, width);
    boolean = malloc(width*width*sizeof(int));
    for(int i = 0;i<width*width;i++)
    {
        if (grid[i] == 0)
            boolean[i] = 0;
        else
            boolean[i] = 1;
    }
    int **result = malloc(width*sizeof(int *));
    for (int i = 0; i<width; i++)
    {
        result[i] = malloc(width*sizeof(int));
        for(int j = 0; j<width; j++)
            result[i][j] = grid[i*width+j];
    }
    res = solve(result, width);
    res += 1;

    int tmp[width*width];
    for(int i =0 ;i<width*width; i++)
        tmp[i] = result[i/width][i%width];
    if(dev_mod)
        print_matrix(tmp, width, width);

    int ** matDigit = get_digit_mat();
    for(int i = 0; i<width; i++)
        for(int j = 0; j <width; j++)
        {
            if(result[i][j] && !grid[i*width+j])
            {
                matrixToSurface(matDigit[result[i][j]-1],bin_surface,
                                bin_surface->w/width*i+5,
                                bin_surface->h/width*j+5,
                                bin_surface->w/width*(i+1)-5,
                                bin_surface->h/width*(j+1)-5);
            }
        }
    for(int i =0; i<width; i++)
        free(matDigit[i]);
    free(matDigit);
    make_thread(&out, bin_surface, "output/treatment/final.png");
    if(dev_mod)
    {
        screen_surface = display_image(bin_surface);
        wait_for_keypressed();
    }
    // ----------------------Free----------------------------------------------
    SDL_FreeSurface(bin_surface);
    SDL_FreeSurface(edge_surface);
    if(dev_mod && screen_surface != NULL)
        SDL_FreeSurface(screen_surface);

    for(size_t i =0; i<out.th; i++)
        pthread_join(out.threads[i], NULL);

    free(out.threads);
    free(out.out);

    for(int i = 0; i < width; i++)
        free(result[i]);
    free(result);
    free(grid);
    free(boolean);
}
