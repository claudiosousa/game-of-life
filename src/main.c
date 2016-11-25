/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */
#include <pthread.h>
#include "stdio.h"
#include "stdlib.h"
#include "display.h"
#include "keyboard.h"
#include "gol_worker.h"

int main(int argc, char* argv[]) {
    if (argc < 7) {
        fprintf(stderr, "Usage: %s <width> <height> <seed> <p> <freq> <#workers>", argv[0]);
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int grid_seed = atoi(argv[3]);
    int grid_p = atoi(argv[4]);
    int refresh_freq = atoi(argv[5]);
    int grid_workers = atoi(argv[6]);
    (void)grid_workers;
    (void)grid_p;
    (void)grid_seed;

    pthread_barrier_t wait_workers, wait_display;
    pthread_barrier_init(&wait_workers, NULL, grid_workers);
    pthread_barrier_init(&wait_display, NULL, grid_workers);

    gol_grid_t* grid = gol_grid_create(width, height);

    gol_worker_t* worker = gol_worker_start(&grid, &wait_workers, &wait_display, grid_workers);
    display_t* dp = display_create("GameOfLife", &grid, refresh_freq, &wait_workers, &wait_display);

    keyboard_create_and_wait_end();

    gol_worker_stop(worker);
    display_stop(dp);
    gol_grid_free(grid);

    pthread_barrier_destroy(&wait_workers);
    pthread_barrier_destroy(&wait_display);

    return EXIT_SUCCESS;
}
