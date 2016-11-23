/**
 * Main program.
 * @author Claudio Sousa, Gonzalez David
 */

#include "stdio.h"
#include "stdlib.h"
#include "display.h"
#include "keyboard.h"

int main(int argc, char * argv[])
{
    if (argc < 7)
    {
        fprintf(stderr, "Usage: %s <width> <height> <seed> <p> <freq> <#workers>", argv[0]);
        return EXIT_FAILURE;
    }

    int screen_width = atoi(argv[1]);
    int screen_height = atoi(argv[2]);
    int grid_seed = atoi(argv[3]);
    int grid_p = atoi(argv[4]);
    int refresh_freq = atoi(argv[5]);
    int grid_worker = atoi(argv[6]);

    display_t * dp = display_create("GameOfLife", screen_width, screen_height, refresh_freq);

    keyboard_sync_t * kb_sync = keyboard_create();
    keyboard_wait_end(kb_sync);

    display_stop(dp);

    return EXIT_SUCCESS;
}
