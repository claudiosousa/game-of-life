/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */
#include "stdio.h"
#include "stdlib.h"
#include "display.h"
#include "keyboard.h"
#include "gol.h"

/**
 * Initialise components and wait till the end
 * @param argc CMD arguments count
 * @param argv CMD arguments array
 * @return Program status
 */
int main(int argc, char* argv[]) {
    if (argc < 7) {
        fprintf(stderr, "Usage: %s <width> <height> <seed> <p> <freq> <#workers>", argv[0]);
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int grid_seed = atoi(argv[3]);
    double grid_p = atof(argv[4]);
    int refresh_freq = atoi(argv[5]);
    int grid_workers = atoi(argv[6]);

    if (grid_workers < 1) {
        fprintf(stderr, "'workers' must be >= 1. Defaulted to 1");
        grid_workers = 1;
    }

    gol_t* gol = gol_create(width, height, grid_seed, grid_p, grid_workers);
    display_t* dp = display_create(gol, refresh_freq);

    keyboard_create_and_wait_end();

    gol_destroy(gol);
    display_stop(dp);

    return EXIT_SUCCESS;
}
