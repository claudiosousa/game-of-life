/**
 * Manage user inputs from another thread
 * @author Claudio Sousa, Gonzalez David
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include "vendor/keyboard_utils.h"
#include "keyboard.h"
#include "time_wait.h"

#define KEYBOARD_EXEC_WAIT_HZ 50

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
        time_wait_freq(NULL, KEYBOARD_EXEC_WAIT_HZ);
    }

    return NULL;
}

/**
 * Create a thread for keyboard and wait for it to end
 */
void keyboard_create_and_wait_end()
{
    pthread_t kb_thread;

    if (pthread_create(&kb_thread, NULL, keyboard_thread, NULL) != 0)
    {
        perror("keyboard thread creation failed");
    }

    if (pthread_join(kb_thread, NULL) != 0)
    {
        perror("keyboard thread join failed");
    }
}
