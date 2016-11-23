/**
 * Manage user inputs from another thread
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

typedef struct keyboard_sync_t keyboard_sync_t;

/**
 * Initialise a keyboard data for synchronisation and start the thread
 * @return Data for synchonisation
 */
keyboard_sync_t * keyboard_create();

/**
 * Wait for the keyboard to terminate and free the allocated data
 * @param kb_sync Synchronisation data to wait on and free afterward
 */
void keyboard_wait_end(keyboard_sync_t * kb_sync);


#endif
