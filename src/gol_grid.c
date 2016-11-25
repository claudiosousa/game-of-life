#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

void gol_grid_copy(gol_grid_t *dst, gol_grid_t *src) {
    if (memcpy(dst->grid, src->grid, src->width * src->height * sizeof(bool)) == NULL)
        perror("Failled to copy grid");
}

void gol_grid_init(gol_grid_t *gol_grid, double seed, double alive_prob) {
    srand(seed);

    for (size_t x = 1; x < gol_grid->width - 1; x++)
        for (size_t y = 1; y < gol_grid->height - 1; y++)
            gol_grid_set_alive(gol_grid, x, y, (double)rand() / RAND_MAX <= alive_prob);
}

static size_t gol_grid_2d_to_1d(gol_grid_t *gol_grid, size_t x, size_t y) { return y * gol_grid->width + x; }

bool gol_grid_get_alive(gol_grid_t *gol_grid, size_t x, size_t y) {
    return gol_grid->grid[gol_grid_2d_to_1d(gol_grid, x, y)];
}

void gol_grid_set_alive(gol_grid_t *gol_grid, size_t x, size_t y, bool alive) {
    gol_grid->grid[gol_grid_2d_to_1d(gol_grid, x, y)] = alive;
}

void gol_grid_get_size(gol_grid_t *gol_grid, size_t *width, size_t *height) {
    *width = gol_grid->width;
    *height = gol_grid->height;
}

void gol_grid_free(gol_grid_t *gol_grid) {
    free(gol_grid->grid);
    free(gol_grid);
}
