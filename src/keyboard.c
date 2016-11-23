/**
 * Manage user inputs from another thread
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "vendor/keyboard_utils.h"
#include "keyboard.h"

struct keyboard_sync_t
{
    pthread_t thread;
};

/**
 * Thread that check user input in order to end the program.
 * This thread tests pressed keys at 50 Hz.
 * @param data Data for synchoisation
 */
void * keyboard_thread(void * data)
{
    (void)data;

    while (keyboard_utils_keypress() != SDLK_ESCAPE)
    {
        usleep(20000);
    }

    return NULL;
}

keyboard_sync_t * keyboard_create()
{
    keyboard_sync_t * kb_sync = malloc(sizeof(keyboard_sync_t));
    if (kb_sync != NULL)
    {
        if (pthread_create(&kb_sync->thread, NULL, keyboard_thread, NULL) != 0)
        {
            perror("keyboard thread creation failed");
        }
    }
    else
    {
        perror("keyboard malloc failed");
    }

    return kb_sync;
}

void keyboard_wait_end(keyboard_sync_t * kb_sync)
{
    if (pthread_join(kb_sync->thread, NULL) != 0)
    {
        perror("keyboard thread join failed");
    }

    free(kb_sync);
}
