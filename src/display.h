/**
 * Display the grid from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef DISPLAY_H
#define DISPLAY_H

typedef struct display_t display_t;

/**
 * Create a display and launch the thread
 * @param window_title
 * @param screen_width
 * @param screen_height
 * @param refresh_freq
 * @return Newly created display for synchonisation purpose
 */
display_t * display_create(char * window_title, int screen_width, int screen_height, int refresh_freq);

/**
 * Stop the display thread and free the resources
 * @param dp Display to stop and free
 */
void display_stop(display_t * dp);

#endif
