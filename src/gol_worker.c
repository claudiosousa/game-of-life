#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "gol_worker.h"

struct gol_worker_t {
    pthread_t *threads;
    gol_grid_t *grid;
    pthread_barrier_t *worker_barrier;
    pthread_barrier_t *display_barrier;
    int workers;
    bool *working;
};

void *gol_worker_thread(void *t_param) {
    gol_worker_t *worker = (gol_worker_t *)t_param;
    while (*worker->working) {
    }
    return NULL;
}

gol_worker_t *gol_worker_start(gol_grid_t *grid, pthread_barrier_t *worker_barrier, pthread_barrier_t *display_barrier,
                               int workers) {
    if (workers < 1) {
        fprintf(stderr, "'workers' must be >= 1. Defaulted to 1");
        workers = 1;
    }
    gol_worker_t *gol_worker = malloc(sizeof(gol_worker_t));
    if (gol_worker == NULL)
        perror("gol_worker_t malloc failed");

    gol_worker->threads = malloc(sizeof(pthread_t) * workers);
    if (gol_worker == NULL)
        perror("gol_worker->threads malloc failed");

    gol_worker->grid = grid;
    gol_worker->worker_barrier = worker_barrier;
    gol_worker->display_barrier = display_barrier;
    gol_worker->workers = workers;
    gol_worker->working = malloc(sizeof(bool));
    *gol_worker->working = true;

    for (int i = 0; i < workers; ++i)
        if (pthread_create(&gol_worker->threads[i], NULL, gol_worker_thread, gol_worker) != 0)
            perror("gol_worker->thread creation failed");

    return gol_worker;
}

void gol_worker_stop(gol_worker_t *gol_worker) {
    *gol_worker->working = false;

    for (int i = 0; i < gol_worker->workers; ++i)
        if (pthread_join(gol_worker->threads[i], NULL) != 0)
            perror("pthread_join gol_worker->thread failed");

    free(gol_worker->working);
    free(gol_worker->threads);
    free(gol_worker);
}
