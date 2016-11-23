#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gfx.h"
#include "keyboard_utils.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

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

	while (keyboard_utils_keypress() != SDLK_ESCAPE) {
		render(ctxt);
		gfx_present(ctxt);
	}

	gfx_destroy(ctxt);
	return EXIT_SUCCESS;
}
