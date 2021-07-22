#include <te.h>

int main() {
	if(te_init("T3Vtech version 2 TEST WINDOW", 1366, 768, 0)!=TE_OKAY) {
		printf("error while initialization\n");
	}

	te_start_renderer(TE_SOFTWARE_RENDERER);

	te_update();
	te_update();
	SDL_Delay(3000);

}