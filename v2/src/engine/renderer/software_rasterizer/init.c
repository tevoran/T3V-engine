#include <te.h>
#include <SDL2/SDL.h>

extern SDL_Window *te_window;

te_software_renderer* te_software_rasterizer_init() {
	printf("Starting the software rasterizer\n");

	te_software_renderer *software_renderer=
		malloc(sizeof(te_software_renderer));

	software_renderer->window=te_window;
	software_renderer->window_surface=SDL_GetWindowSurface(te_window);
	software_renderer->resx=software_renderer->window_surface->w;
	software_renderer->resy=software_renderer->window_surface->h;
	
	printf("Using resolution %ix%i\n",
		software_renderer->resx,
		software_renderer->resy);

	return software_renderer;
}
