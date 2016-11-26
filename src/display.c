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
    struct gfx_context_t *ctxt;
    int screen_width;
    int screen_height;
    int refresh_freq;
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
        time_start(&tm);

        for (int x = 0; x < dp->screen_width; ++x)
            for (int y = 0; y < dp->screen_height; ++y)
                gfx_putpixel(dp->ctxt, x, y, gol_is_alive(dp->gol, x, y) ? COLOR_WHITE : COLOR_BLACK);

        gfx_present(dp->ctxt);

        time_wait_freq(&tm, dp->refresh_freq);

        // printf("display -\n");
        gol_work_sync(dp->gol);
        //printf("display --\n");

        if (!gol_is_running(dp->gol))
            break;
        gol_work_sync(dp->gol);
    } while (true);

    return NULL;
}

display_t *display_create(char *window_title, gol_t *gol, int refresh_freq) {
    display_t *dp = malloc(sizeof(display_t));

    if (dp != NULL) {
        size_t screen_width, screen_height;
        gol_get_size(gol, &screen_width, &screen_height);

        dp->ctxt = gfx_create(window_title, screen_width, screen_height);
        dp->gol = gol;
        dp->screen_width = screen_width;
        dp->screen_height = screen_height;
        dp->refresh_freq = refresh_freq;

        if (pthread_create(&dp->thread, NULL, display_thread, dp) != 0) {
            perror("display thread creation failed");
        }
    } else {
        perror("display malloc failed");
    }

    return dp;
}

void display_stop(display_t *dp) {
    if (pthread_join(dp->thread, NULL) != 0) {
        perror("display thread join failed");
    }

    gfx_destroy(dp->ctxt);
    free(dp);
}
