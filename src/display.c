/**
 * Display the grid from another thread.
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdbool.h>
#include <pthread.h>
#include "vendor/gfx.h"
#include "display.h"
#include "time.h"

struct display_t
{
    struct gfx_context_t * ctxt;
    int screen_width;
    int screen_height;
    int refresh_freq;

    bool running;
    pthread_t thread;
};

/**
 * Display thread that show the current generated grid
 * @param data Display data
 */
void * display_thread(void * data)
{
    display_t * dp = (display_t*)data;
    struct timespec tm;

    while (dp->running)
    {
        time_start(&tm);

        for (int i = 0; i < dp->ctxt->width*dp->ctxt->height/10; i++) {
            int x = rand() % dp->ctxt->width;
            int y = rand() % dp->ctxt->height;
            uint32_t color = MAKE_COLOR(rand() % 256,rand() % 256,rand() % 256);
            gfx_putpixel(dp->ctxt, x, y, color);
        }
        gfx_present(dp->ctxt);

        time_wait_freq(&tm, dp->refresh_freq);
    }

    return NULL;
}

display_t * display_create(char * window_title, int screen_width, int screen_height, int refresh_freq)
{
    display_t * dp = malloc(sizeof(display_t));
    if (dp != NULL)
    {
        dp->ctxt = gfx_create(window_title, screen_width, screen_height);
        dp->screen_width = screen_width;
        dp->screen_height = screen_height;
        dp->refresh_freq = refresh_freq;
        dp->running = true;

        if (pthread_create(&dp->thread, NULL, display_thread, dp) != 0)
        {
            perror("display thread creation failed");
        }
    }
    else
    {
        perror("display malloc failed");
    }

    return dp;
}

void display_stop(display_t * dp)
{
    dp->running = false;
    if (pthread_join(dp->thread, NULL) != 0)
    {
        perror("display thread join failed");
    }

    gfx_destroy(dp->ctxt);
    free(dp);
}
