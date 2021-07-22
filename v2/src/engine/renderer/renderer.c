#include <te.h>

unsigned long te_current_renderer=TE_NO_RENDERER;
te_software_renderer *software_renderer=NULL;

int te_start_renderer(unsigned long FLAGS) {
	if(FLAGS & TE_SOFTWARE_RENDERER) {
		printf("Selecting the T3Vtech software renderer\n");
		te_current_renderer=TE_SOFTWARE_RENDERER;

		//starting the software rasterizer
		software_renderer=te_software_rasterizer_init();
	}
}

void te_update() {
	if(te_current_renderer==TE_SOFTWARE_RENDERER) {
		te_software_rasterizer_update(software_renderer);
	}
}


//cleaning up
void te_quit() {
	if(software_renderer!=NULL) {
		free(software_renderer);
	}
}
