/**
 * Display the gol from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdbool.h>
#include <pthread.h>
#include "vendor/gfx.h"
#include "display.h"
#include "time_wait.h"

#define DISPLAY_WINDOW_NAME "GameOfLife"

struct display_t {
    int refresh_freq;

    gol_t *gol;
    pthread_t thread;
    pthread_barrier_t init_sync;
};

/**
 * Display thread that show the current generated gol
 * @param data Display data
 */
void *display_thread(void *data) {
    display_t *dp = (display_t *)data;
    struct timespec tm;

    size_t screen_width;
    size_t screen_height;
    gol_get_size(dp->gol, &screen_width, &screen_height);

    // gfx_create() and gfx_present() must be in the same thread
    struct gfx_context_t *ctxt = gfx_create(DISPLAY_WINDOW_NAME, screen_width, screen_height);
    if (ctxt == NULL)
    {
        fprintf(stderr, "Display context creation failed.");
        return NULL;
    }

    // Join the barrier in order to tell other component that
    // the display has been fully initialised
    display_sync_init(dp);

    do {
        time_wait_start(&tm);

        for (size_t x = 0; x < screen_width; ++x)
            for (size_t y = 0; y < screen_height; ++y)
                gfx_putpixel(ctxt, x, y, gol_is_cell_alive(dp->gol, x, y) ? COLOR_WHITE : COLOR_BLACK);

        gfx_present(ctxt);

        gol_work_sync(dp->gol);

        if (!gol_is_running(dp->gol))
            break;

        gol_work_sync(dp->gol);

        time_wait_freq(&tm, dp->refresh_freq);
    } while (true);

    gfx_destroy(ctxt);

    return NULL;
}

/**
 * Create a display and launch the thread
 * @param gol Game of life to display
 * @param refresh_freq How many times per second the screen should refresh
 * @param other_component_nb Number of component that depend on the complete initialisation of the display module
 * @return Newly created display for synchonisation purpose
 */
display_t *display_create(gol_t *gol, int refresh_freq, unsigned int other_component_nb) {
    display_t *dp = malloc(sizeof(display_t));
    if (dp == NULL) {
        perror("display malloc failed");
        return NULL;
    }

    dp->gol = gol;
    dp->refresh_freq = refresh_freq;

    if (pthread_barrier_init(&dp->init_sync, NULL, other_component_nb + 1) != 0) {
        perror("display barrier init failed");
        free(dp);
        return NULL;
    }

    if (pthread_create(&dp->thread, NULL, display_thread, dp) != 0) {
        perror("display thread creation failed");
        free(dp);
        return NULL;
    }

    return dp;
}

/**
 * Stop the display thread and free the resources
 * @param dp Display to stop and free
 */
void display_stop(display_t *dp) {
    if (pthread_join(dp->thread, NULL) != 0) {
        perror("display thread join failed");
    }

    if (pthread_barrier_destroy(&dp->init_sync) != 0) {
        perror("display barrier destroy failed");
    }

    free(dp);
}

/**
 * Help the synchonisation of all element that depend on display
 * @param dp Display data to synchronise
 */
void display_sync_init(display_t *dp) {
    pthread_barrier_wait(&dp->init_sync);
}
