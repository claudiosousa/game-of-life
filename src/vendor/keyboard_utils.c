/**
 * Provided keyboard functions.
 * @author Claudio Sousa, Gonzalez David
 */

#include <SDL2/SDL.h>
#include "keyboard_utils.h"

int keyboard_utils_keypress() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}
