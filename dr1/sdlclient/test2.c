#include <stdio.h>
#include <SDL.h>

int main( int argc, char **argv) {
    char buf[80];
    SDL_VideoInfo *vid;
    
    /* Initialize graphics */
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	exit(1);
    }
    atexit(SDL_Quit);
    vid = SDL_GetVideoInfo();
    printf("HW available: %s\n", vid->hw_available?"Yes":"No");
    printf("WM available: %s\n", vid->wm_available?"Yes":"No");
    printf("blit_hw: %s\n", vid->blit_hw?"Yes":"No");
    printf("blit_hw_CC: %s\n", vid->blit_hw_CC?"Yes":"No");
    printf("blit_hw_A: %s\n", vid->blit_hw_A?"Yes":"No");
    printf("blit_sw: %s\n", vid->blit_sw?"Yes":"No");
    printf("blit_sw_CC: %s\n", vid->blit_sw_CC?"Yes":"No");
    printf("blit_sw_A: %s\n", vid->blit_sw_A?"Yes":"No");
    printf("blit_fill: %d\n", vid->blit_fill);
    printf("video_mem: %d", vid->video_mem);
}
