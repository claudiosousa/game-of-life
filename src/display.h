/**
 * Display the gol from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "gol.h"

typedef struct display_t display_t;

display_t *display_create(char *window_title, gol_t *gol, int refresh_freq);

void display_stop(display_t *dp);

#endif
