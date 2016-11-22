#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gfx.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
static SDL_Keycode keypress() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
			return event.key.keysym.sym;
	}
	return 0;
}

/// Render some white noise.
/// @param context graphical context to use.
static void render(struct gfx_context_t *context) {
	gfx_clear(context, COLOR_BLACK);

	for (int i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT/10; i++) {
		int x = rand() % context->width;
		int y = rand() % context->height;
		uint32_t intensity = rand() % 256;  // 8-bit per color channel
		uint32_t color = MAKE_COLOR(intensity,intensity,intensity);
		gfx_putpixel(context, x, y, color);
	}
}

/// Program entry point.
/// @return the application status code (0 if success).
int main() {
	struct gfx_context_t *ctxt = gfx_create("Example", SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!ctxt) {
		fprintf(stderr, "Graphic mode initialization failed!\n");
		return EXIT_FAILURE;
	}

	while (keypress() != SDLK_ESCAPE) {
		render(ctxt);
		gfx_present(ctxt);
	}

	gfx_destroy(ctxt);
	return EXIT_SUCCESS;
}
