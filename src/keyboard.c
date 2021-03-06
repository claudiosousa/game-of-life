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
 * @param data None
 * @return None
 */
void* keyboard_thread(void* data) {
    (void)data;
    struct timespec tm;
    time_wait_start(&tm);

    while (keyboard_utils_keypress() != SDLK_ESCAPE) {
        time_wait_freq(&tm, KEYBOARD_EXEC_WAIT_HZ);
     }

    return NULL;
}

/**
 * Create a thread for keyboard and wait for it to end
 * @return 0 for OK, 1 for NOK
 */
int keyboard_create_and_wait_end() {
    pthread_t kb_thread;

    if (pthread_create(&kb_thread, NULL, keyboard_thread, NULL) != 0) {
        perror("keyboard thread creation failed");
        return 1;
    }

    if (pthread_join(kb_thread, NULL) != 0) {
        perror("keyboard thread join failed");
        return 1;
    }

    return 0;
}
