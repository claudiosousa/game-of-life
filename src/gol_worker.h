typedef struct gol_worker_t gol_worker_t;
gol_worker_t * gol_worker_start(gol_t *gol_grid, barrier_workers, barrier_display, bool *working);
void gol_worker_stop(gol_worker_t * gol_worker);
