/**
 * Provided keyboard functions.
 * @author Claudio Sousa, Gonzalez David
 */

#include <SDL2/SDL.h>
#include "keyboard_utils.h"

/**
 * If a key was pressed, returns its key code (non blocking call).
 * List of key codes: https://wiki.libsdl.org/SDL_Keycode
 * WARNING: https://wiki.libsdl.org/SDL_PollEvent#Remarks
 *          https://wiki.libsdl.org/SDL_PumpEvents#Remarks
 * @return the key that was pressed or 0 if none was pressed.
 */
int keyboard_utils_keypress() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}
