#ifndef PROCESS_H_
#define PROCESS_H_

#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "utils/init.h"
#include "utils/scale.h"
#include "utils/resize.h"
#include "utils/thread.h"
#include "utils/mat_digit.h"

#include "color_treatment/smoothing.h"
#include "color_treatment/increase_contrast.h"
#include "color_treatment/blur.h"
#include "color_treatment/noise.h"
#include "color_treatment/grayscale.h"
#include "color_treatment/brightness.h"
#include "color_treatment/adaptive_threshold.h"
#include "color_treatment/edge_detection.h"
#include "color_treatment/inverse.h"
#include "color_treatment/bold.h"

#include "detection/grid_detection.h"
#include "detection/hough_transform.h"
#include "detection/distorsion.h"

#include "slot_processing/slicing.h"
#include "slot_processing/redraw.h"

#include "neural_network/neural_network.h"

#include "solver/solver.h"

struct carth{
    int x;
    int y;
};

typedef struct{
    char* file;
    int hexa;
    int dev_mod;
}process_thread;


void process(char* path, int hexa, int dev_mode);

#endif
