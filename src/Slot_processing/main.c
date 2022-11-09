#include <err.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "resize.h"
#include "init.h"
#include "slicing.h"

int main(int argc, char* argv[])
{
  if(argc != 2)
    errx(1, "Type wanted is : ./out file.png\n");
  
  SDL_Surface* main_surface;

  init_sdl();

  main_surface = load_image(argv[1]);
  slicing(main_surface);
  
  return 0;
}
