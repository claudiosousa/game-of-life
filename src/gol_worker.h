/**
 * Creates worker threads to process a gol_grid
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef GOL_WORKER_H
#define GOL_WORKER_H

#include "gol_grid.h"

typedef struct gol_worker_t gol_worker_t;
gol_worker_t *gol_worker_start(gol_grid_t *grid, pthread_barrier_t *worker_barrier, pthread_barrier_t *display_barrier,
                               int workers);
void gol_worker_stop(gol_worker_t *gol_worker);

#endif