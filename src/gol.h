/**
 * Manipulates a memory representation of the GoL.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef GOL_H
#define GOL_H

#include <stdbool.h>

typedef struct gol_t gol_t;

gol_t *gol_create(size_t width, size_t height, double seed, double alive_prob, int workers);
bool gol_is_alive(gol_t *gol, size_t x, size_t y);
bool gol_is_running(gol_t *gol);
void gol_work_sync(gol_t *gol);
void gol_get_size(gol_t *gol, size_t *x, size_t *y);
void gol_destroy(gol_t *gol);

#endif
