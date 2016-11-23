/**
 * Provided keyboard functions.
 * @author Claudio Sousa, Gonzalez David
 */

#ifndef KEYBOARD_UTILS_H
#define KEYBOARD_UTILS_H

/**
 * If a key was pressed, returns its key code (non blocking call).
 * List of key codes: https://wiki.libsdl.org/SDL_Keycode
 * @return the key that was pressed or 0 if none was pressed.
 */
int keyboard_utils_keypress();

#endif
