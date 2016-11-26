#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gol.h"

struct gol_t {
    // could be replaced by bitwise operations on word[]
    bool *grid;
    bool *temp_grid;
    size_t width;
    size_t height;

    pthread_t *threads;
    pthread_barrier_t finished_sync;
    pthread_mutex_t *rendering_state_lock;
    bool frame_rendered;
    int workers;
    bool running;
};

typedef struct {
    gol_t *gol;
    int index;
    int size;
} gol_worker_t;

static size_t gol_2d_to_1d(gol_t *gol, size_t x, size_t y) {
    return y * gol->width + x;
}

static void gol_init(gol_t *gol, double seed, double alive_prob) {
    srand(seed);

    memset(gol->grid, 0, gol->width * gol->height * sizeof(bool));

    for (size_t x = 1; x < gol->width - 1; x++)
        for (size_t y = 1; y < gol->height - 1; y++)
            if ((double)rand() / RAND_MAX <= alive_prob)
                gol->grid[gol_2d_to_1d(gol, x, y)] = true;
}

void gol_work_sync(gol_t *gol) {
    pthread_barrier_wait(&gol->finished_sync);
}

bool gol_is_running(gol_t *gol) {
    return gol->running;
}

static void gol_set_frame_rendered_state(gol_t *gol, bool rendered) {
    if (pthread_mutex_lock(gol->rendering_state_lock) != 0)
        perror("pthread_mutex_lock failed");

    if (rendered && !gol->frame_rendered) {
        bool *t = gol->grid;
        gol->grid = gol->temp_grid;
        gol->temp_grid = t;
    }
    gol->frame_rendered = rendered;

    if (pthread_mutex_unlock(gol->rendering_state_lock) != 0)
        perror("pthread_mutex_unlock failed");
}

static void gol_update_cells(gol_worker_t *worker) {
    for (int i = worker->index; i < worker->size; i += worker->gol->workers) {
        int neighbours = 0;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                if (x == 0 && y == 0)
                    continue;
                if (worker->gol->grid[(i + y * worker->gol->width + x) % worker->size])
                    neighbours++;
            }
        }
        if (worker->gol->grid[i]) {
            worker->gol->temp_grid[i] = neighbours == 2 || neighbours == 3;
        } else
            worker->gol->temp_grid[i] = neighbours == 3;
    }
}

static void *gol_work_thread(void *t_param) {
    gol_worker_t *worker = (gol_worker_t *)t_param;
    do {
        gol_set_frame_rendered_state(worker->gol, false);

        gol_update_cells(worker);

        // printf("Thread - %d\n", worker->index);
        gol_work_sync(worker->gol);  // sync end
        // printf("Thread -- %d\n", worker->index);
        if (!worker->gol->running)
            break;
        gol_set_frame_rendered_state(worker->gol, true);
        gol_work_sync(worker->gol);  // sync start
    } while (true);

    free(worker);
    return NULL;
}

static void gol_start_workers(gol_t *gol) {
    gol->threads = malloc(sizeof(pthread_t) * gol->workers);
    if (gol->threads == NULL)
        perror("gol->threads malloc failed");

    for (int i = 0; i < gol->workers; ++i) {
        gol_worker_t *worker = malloc(sizeof(gol_worker_t));
        if (worker == NULL)
            perror("worker malloc failed");
        worker->gol = gol;
        worker->index = i;
        worker->size = gol->width * gol->height;

        if (pthread_create(&gol->threads[i], NULL, gol_work_thread, worker) != 0)
            perror("gol->thread creation failed");
    }
}

static void gol_stop_workers(gol_t *gol) {
    gol->running = false;

    for (int i = 0; i < gol->workers; ++i)
        if (pthread_join(gol->threads[i], NULL) != 0)
            perror("pthread_join gol->thread failed");
}

gol_t *gol_create(size_t width, size_t height, double seed, double alive_prob, int workers) {
    gol_t *gol = (gol_t *)malloc(sizeof(gol_t));
    if (gol == NULL)
        perror("gol_t malloc failed");

    size_t grid_mem_size = sizeof(bool) * width * height;
    gol->width = width;
    gol->height = height;
    gol->workers = workers;
    gol->running = true;
    gol->grid = (bool *)malloc(grid_mem_size);
    gol->temp_grid = (bool *)malloc(grid_mem_size);
    gol->rendering_state_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

    if (gol->grid == NULL || gol->temp_grid == NULL || gol->rendering_state_lock == NULL)
        perror("gol malloc failed");

    if (pthread_mutex_init(gol->rendering_state_lock, NULL) != 0)
        perror("pthread_mutex_init failed");

    if (pthread_barrier_init(&gol->finished_sync, NULL, workers + 1) != 0)
        perror("pthread_barrier_init failed");

    gol_init(gol, seed, alive_prob);
    gol_start_workers(gol);

    return gol;
}

bool gol_is_alive(gol_t *gol, size_t x, size_t y) {
    return gol->grid[gol_2d_to_1d(gol, x, y)];
}

void gol_get_size(gol_t *gol, size_t *width, size_t *height) {
    *width = gol->width;
    *height = gol->height;
}

void gol_destroy(gol_t *gol) {
    gol_stop_workers(gol);
    if (pthread_barrier_destroy(&gol->finished_sync) != 0)
        perror("pthread_barrier_destroy failed");

    free(gol->threads);
    free(gol->grid);
    free(gol->temp_grid);
    free(gol);
}
