/**
 * Display the gol from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "gol.h"

typedef struct display_t display_t;

/**
 * Create a display and launch the thread
 * @param window_title
 * @param screen_width
 * @param screen_height
 * @param refresh_freq
 * @return Newly created display for synchonisation purpose
 */
display_t *display_create(char *window_title, gol_t *gol, int refresh_freq);

/**
 * Stop the display thread and free the resources
 * @param dp Display to stop and free
 */
void display_stop(display_t *dp);

#endif
