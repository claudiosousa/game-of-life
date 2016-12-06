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
        fprintf(stderr, "Usage:   %s <width> <height> <seed> <p> <freq> <#workers>\n", argv[0]);
        fprintf(stderr, "Example: %s 240 135 0 0.75 30 8\n", argv[0]);
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int grid_seed = atoi(argv[3]);
    double grid_p = atof(argv[4]);
    int refresh_freq = atoi(argv[5]);
    int grid_workers = atoi(argv[6]);

    if (grid_workers < 1) {
        fprintf(stderr, "'workers' must be >= 1. Defaulted to 1\n");
        grid_workers = 1;
    }

    if (refresh_freq < 1) {
        fprintf(stderr, "'refresh_freq' must be >= 1. Defaulted to 1\n");
        refresh_freq = 1;
    }

    gol_t* gol = gol_create(width, height, grid_seed, grid_p, grid_workers);
    if (gol == NULL) {
        return EXIT_FAILURE;
    }
    display_t* dp = display_create(gol, refresh_freq);
    if (dp == NULL) {
        return EXIT_FAILURE;
    }

    if (keyboard_create_and_wait_end() != 0) {
        return EXIT_FAILURE;
    }

    gol_stop(gol);
    display_stop(dp);

    gol_free(gol);

    return EXIT_SUCCESS;
}
