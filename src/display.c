/**
 * Display the gol from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdbool.h>
#include <pthread.h>
#include "vendor/gfx.h"
#include "display.h"
#include "time_wait.h"

struct display_t {
    size_t screen_width;
    size_t screen_height;
    int refresh_freq;

    struct gfx_context_t *ctxt;
    gol_t *gol;
    pthread_t thread;
};

/**
 * Display thread that show the current generated gol
 * @param data Display data
 */
void *display_thread(void *data) {
    display_t *dp = (display_t *)data;
    struct timespec tm;

    do {
        time_wait_start(&tm);

        for (size_t x = 0; x < dp->screen_width; ++x)
            for (size_t y = 0; y < dp->screen_height; ++y)
                gfx_putpixel(dp->ctxt, x, y, gol_is_alive(dp->gol, x, y) ? COLOR_WHITE : COLOR_BLACK);

        gfx_present(dp->ctxt);

        time_wait_freq(&tm, dp->refresh_freq);

        gol_work_sync(dp->gol);

        if (!gol_is_running(dp->gol))
            break;

        gol_work_sync(dp->gol);
    } while (true);

    return NULL;
}

/**
 * Create a display and launch the thread
 * @param window_title Title of the created window
 * @param gol Game of life to display
 * @param refresh_freq How many times per second the screen should refresh
 * @return Newly created display for synchonisation purpose
 */
display_t *display_create(char *window_title, gol_t *gol, int refresh_freq) {
    display_t *dp = malloc(sizeof(display_t));

    if (dp != NULL) {
        gol_get_size(gol, &dp->screen_width, &dp->screen_height);

        dp->ctxt = gfx_create(window_title, dp->screen_width, dp->screen_height);
        dp->gol = gol;
        dp->refresh_freq = refresh_freq;

        if (pthread_create(&dp->thread, NULL, display_thread, dp) != 0) {
            perror("display thread creation failed");
        }
    } else {
        perror("display malloc failed");
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

    gfx_destroy(dp->ctxt);
    free(dp);
}
