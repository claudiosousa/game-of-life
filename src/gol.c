/**
 * Manipulates a memory representation of the GoL.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "gol.h"

struct gol_t {
    bool *grid;
    bool *temp_grid;
    size_t width;
    size_t height;

    pthread_t *threads;
    pthread_barrier_t work_sync;
    int workers;
    bool stop;
    bool request_stop;
};

typedef struct {
    gol_t *gol;
    int index;
    int size;
} gol_worker_t;

/**
 * Transform a 2D coordinate into a 1D one
 * @param gol GoL data
 * @param x First coordiate in 2D
 * @param y Second coordiate in 2D
 * @return 1D coordinate
 */
static size_t gol_2d_to_1d(gol_t *gol, size_t x, size_t y) {
    return y * gol->width + x;
}

/**
 * Transform a 1D coordinate into 2D
 * @param gol GoL data
 * @param index The 1D coordinate
 * @param x First coordiate in 2D
 * @param y Second coordiate in 2D
 */
static void gol_1d_to_2d(gol_t *gol, size_t index, size_t *x, size_t *y) {
    *x = index % gol->width;
    *y = index / gol->width;
}

/**
 * Randomly initialize a gol grid
 * @param gol GoL data
 * @param seed Seed for the RNG
 * @param alive_prob Probality to have an alive cell
 */
static void gol_init(gol_t *gol, double seed, double alive_prob) {
    srand(seed);

    memset(gol->grid, 0, gol->width * gol->height * sizeof(bool));

    for (size_t y = 1; y < gol->height - 1; y++)
        for (size_t x = 1; x < gol->width - 1; x++)
            if ((double)rand() / RAND_MAX <= alive_prob)
                gol->grid[gol_2d_to_1d(gol, x, y)] = true;
}

/**
 * Swap gol finished grid and gol working temporary
 * @param gol GoL data
 */
static void gol_swap_working_grid(gol_t *gol) {
    bool *t = gol->grid;
    gol->grid = gol->temp_grid;
    gol->temp_grid = t;
}

/**
 * Get a cell neighbours
 * @param gol The GoL data
 * @param cell_index The cell index
 * @return the number of alive neighbours, -1 if border cell
 */
static int gol_cell_neightbours(gol_t *gol, int cell_index) {
    size_t initial_x, initial_y;

    gol_1d_to_2d(gol, cell_index, &initial_x, &initial_y);
    if (initial_x == 0 || initial_x == gol->width - 1 || initial_y == 0 || initial_y == gol->height - 1)
        return -1;

    int neighbours = 0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            if (x == 0 && y == 0)
                continue;
            if (gol->grid[gol_2d_to_1d(gol, initial_x + x, initial_y + y)])
                neighbours++;
        }
    }

    return neighbours;
}

/**
 * Update all cells by firstly getting the cell neighbours and then applying rules
 * @param worker Worker data, containing the GoL data
 */
static void gol_update_cells(gol_worker_t *worker) {
    for (int i = worker->index; i < worker->size; i += worker->gol->workers) {
        int neighbours = gol_cell_neightbours(worker->gol, i);
        if (neighbours < 0)
            continue;

        if (worker->gol->grid[i]) {
            worker->gol->temp_grid[i] = neighbours == 2 || neighbours == 3;
        } else
            worker->gol->temp_grid[i] = neighbours == 3;
    }
}

/**
 * The thread function that treat the GoL with synchronisation
 * @param t_param Worker data, containing the GoL data
 * @return NULL
 */
static void *gol_work_thread(void *t_param) {
    gol_worker_t *worker = (gol_worker_t *)t_param;
    do {
        gol_update_cells(worker);

        if (worker->gol->request_stop)
            worker->gol->stop = true;

        gol_work_sync(worker->gol);  // sync end

        if (worker->gol->stop)
            break;
        if (worker->index == 0)
            gol_swap_working_grid(worker->gol);

        gol_work_sync(worker->gol);  // sync start
    } while (true);

    free(worker);
    return NULL;
}

/**
 * Create the worker for parallel GoL treatment
 * @param gol GoL data
 * @return 0 for OK, 1 for NOK
 */
static int gol_start_workers(gol_t *gol) {
    gol->threads = malloc(sizeof(pthread_t) * gol->workers);
    if (gol->threads == NULL) {
        perror("gol->threads malloc failed");
        return 1;
    }

    for (int i = 0; i < gol->workers; ++i) {
        gol_worker_t *worker = malloc(sizeof(gol_worker_t));
        if (worker == NULL) {
            perror("worker malloc failed");
            return 1;
        }

        worker->gol = gol;
        worker->index = i;
        worker->size = gol->width * gol->height;

        if (pthread_create(&gol->threads[i], NULL, gol_work_thread, worker) != 0) {
            perror("gol->thread creation failed");
            return 1;
        }
    }

    return 0;
}

/**
 * Create a GoL and launch threads
 * @param width Grid width
 * @param height Grid height
 * @param seed Seed for the RNG
 * @param alive_prob Probality to have an alive cell
 * @param workers Number of threads that will treat the GoL
 * @return GoL data
 */
gol_t *gol_create(size_t width, size_t height, double seed, double alive_prob, int workers) {
    gol_t *gol = (gol_t *)malloc(sizeof(gol_t));
    if (gol == NULL) {
        perror("gol_t malloc failed");
        return NULL;
    }

    size_t grid_mem_size = sizeof(bool) * width * height;
    gol->width = width;
    gol->height = height;
    gol->workers = workers;
    gol->stop = gol->request_stop = false;
    gol->grid = (bool *)malloc(grid_mem_size);
    gol->temp_grid = (bool *)malloc(grid_mem_size);

    if (gol->grid == NULL || gol->temp_grid == NULL) {
        perror("gol malloc failed");
        return NULL;
    }

    if (pthread_barrier_init(&gol->work_sync, NULL, workers + 1) != 0) {
        perror("pthread_barrier_init failed");
        return NULL;
    }

    gol_init(gol, seed, alive_prob);
    if (gol_start_workers(gol) != 0) {
        return NULL;
    }

    return gol;
}

/**
 * In a given position, determine if a cell is alive
 * @param gol GoL data
 * @param x X coordinate
 * @param y Y coordinate
 * @return TRUE for alive, FALSE for dead
 */
bool gol_is_cell_alive(gol_t *gol, size_t x, size_t y) {
    return gol->grid[gol_2d_to_1d(gol, x, y)];
}

/**
 * Tell if the GoL treatment should continue
 * @param gol GoL data
 * @return TRUE for continue, FALSE for terminate
 */
bool gol_is_running(gol_t *gol) {
    return !gol->stop;
}

/**
 * Synchronizes all threads working on the gol grid
 * Both for gol workers & for display thread
 * @param gol GoL data
 */
void gol_work_sync(gol_t *gol) {
    pthread_barrier_wait(&gol->work_sync);
}

/**
 * Get the size of a GoL
 * @param gol GoL data
 * @param width Memory store for the resulting width
 * @param height Memory store for the resulting height
 */
void gol_get_size(gol_t *gol, size_t *width, size_t *height) {
    *width = gol->width;
    *height = gol->height;
}

/**
 * Signal the threads to stop and wait for them to terminate gracefully
 * @param gol GoL data
 */
void gol_stop(gol_t *gol) {
    gol->request_stop = true;

    for (int i = 0; i < gol->workers; ++i)
        if (pthread_join(gol->threads[i], NULL) != 0)
            perror("pthread_join gol->thread failed");
}

/**
 * Free all resources of a GoL
 * @param gol GoL data
 */
void gol_free(gol_t *gol) {
    if (pthread_barrier_destroy(&gol->work_sync) != 0)
        perror("pthread_barrier_destroy failed");

    free(gol->threads);
    free(gol->grid);
    free(gol->temp_grid);
    free(gol);
}
