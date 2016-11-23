#include <stdlib.h>
#include "gol_grid.h"

struct gol_grid_t {
    // could be replaced by bitwise operations on word[]
    bool *grid;
    size_t width;
    size_t height;
};

gol_grid_t *gol_grid_create(size_t width, size_t height) {
    gol_grid_t *gol_grid = (gol_grid_t *)malloc(sizeof(gol_grid_t));
    if (gol_grid == NULL)
        perror("gol_grid_t malloc failed");

    gol_grid->width = width;
    gol_grid->height = height;
    gol_grid->grid = (bool *)malloc(sizeof(bool) * width * height);
    if (gol_grid->grid == NULL)
        perror("grid malloc failed");

    return gol_grid;
}

void gol_grid_copy(gol_grid_t *gol_grid_dst, gol_grid_t *gol_grid_src) {
    if (memcpy(gol_grid_dst->grid, gol_grid_dst->grid, width * height * sizeof(bool)) == NULL)
        perror("Failled to copy grids");
}

void gol_grid_init(gol_grid_t *gol_grid, double seed, double alive_prob) {
    srand(seed);

    for (int x = 1; x < gol_grid->width - 1; x++) {
        for (int y = 1; y < gol_grid->height - 1; y++) {
            gol_grid_set_state(gol_grid, x, y, ((double)rand() / RAND_MAX) >= alive_prob)
        }
    }
}

// bool gol_grid_get_next_state(gol_t *gol_grid, double seed, double alive_prob);
bool gol_grid_is_alive(gol_grid_t *gol_grid, size_t x, size_t y);
void gol_grid_get_size(gol_grid_t *gol_grid, size_t *x, size_t *y);
// neightbours, set_state, get_state
void gol_grid_free(gol_grid_t *gol_grid)
