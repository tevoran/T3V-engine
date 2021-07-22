#ifndef TE_SOFTWARE_RASTERIZER_H
#define TE_SOFTWARE_RASTERIZER_H

#include <SDL2/SDL.h>

//using the struct as a collection of date for the software renderer
struct te_software_renderer {
	SDL_Window *window;
	SDL_Surface *window_surface;
	int resx;
	int resy;
} typedef te_software_renderer;

te_software_renderer* te_software_rasterizer_init();
void te_software_rasterizer_update(te_software_renderer *software_renderer);


void te_software_rasterizer_draw_pixel(
	SDL_Color *pixel_ptr, 
	SDL_Color pixel_color);


#endif