#include <stdio.h>
#include <SDL.h>

#include "map.h"

SDL_Surface *dr1_npcs1;
SDL_Surface *dr1_npcs2;
SDL_Surface *dr1_npcs3;
SDL_Surface *dr1_dung1;
SDL_Surface *dr1_dung2;
SDL_Surface *dr1_dung3;
SDL_Surface *dr1_castle1;
SDL_Surface *dr1_objects1;

SDL_Surface *LoadBMP(char *file, SDL_Surface *screen)
{
    SDL_Surface *image;
    SDL_Surface *display_image;
    SDL_Rect dest;
    SDL_PixelFormat *fmt = screen->format;
    Uint32 key = SDL_MapRGB( fmt, 255, 0, 255);

    /* Load the BMP file into a surface */
    image = SDL_LoadBMP(file);
    if ( image == NULL ) {
	fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
	return;
    }
    display_image = SDL_DisplayFormat( image);
    SDL_SetColorKey( display_image, SDL_SRCCOLORKEY, key);
    return display_image;
}


void blit24x35(
    SDL_Surface *display_image, 
    int row, int col,
    SDL_Surface *screen, 
    int x, int y)
{
    SDL_Rect src, dest;
    /* Blit onto the screen surface.
       The surfaces should not be locked at this point.
     */
     
    src.x = col * 27 + 2;
    src.y = row * 38 + 1;
    src.w = 24;
    src.h = 35;
    dest.x = x;
    dest.y = y;
    dest.w = 24;
    dest.h = 35;
    SDL_BlitSurface(display_image, &src, screen, &dest);

#if 0
    SDL_UpdateRects(screen, 1, &dest);
#endif
#if 0
    SDL_Flip( screen);
#endif
}

int main( int argc, char **argv) {
    char buf[80];
    
    /* Initialize graphics */
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	exit(1);
    }
    atexit(SDL_Quit);

    { SDL_Surface *screen;
        dr1Map map;

#if 1
	screen = SDL_SetVideoMode(640, 480, 16, SDL_SWSURFACE);
#else
	screen = SDL_SetVideoMode(640, 480, 16, 
	    SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
#endif
	if ( screen == NULL ) {
	    fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
	    exit(1);
	}

	dr1_npcs1 = LoadBMP( GFXDIR "/db-npcs-1.bmp", screen);
	dr1_npcs2 = LoadBMP( GFXDIR "/db-npcs-2.bmp", screen);
	dr1_npcs3 = LoadBMP( GFXDIR "/db-npcs-3.bmp", screen);
	dr1_dung1 = LoadBMP( GFXDIR "/db-indoor-dungeon-1.bmp", screen);
	dr1_dung2 = LoadBMP( GFXDIR "/db-indoor-dungeon-2.bmp", screen);
	dr1_dung3 = LoadBMP( GFXDIR "/db-indoor-dungeon-3.bmp", screen);
	dr1_castle1 = LoadBMP( GFXDIR "/db-indoor-castle-1.bmp", screen);
	dr1_objects1 = LoadBMP( GFXDIR "/db-objects-1.bmp", screen);

        map = readmap( "map.txt");

	{
	    SDL_Event event;
	    int r,c,g;
	    SDL_Surface *img = dr1_npcs1;

	    for (;;) {
	        SDL_FillRect( screen, NULL, 0);
		for (r=0; r<map.ysize; r++) {
		    for (c=0; c<map.xsize; c++) {	    
		        dr1MapGraphic *gr = &map.graphics[ r*map.xsize +c];
		        for (g=gr->nglyphs-1; g>=0; g--) {
			    blit24x35( gr->glyph[g].src, gr->glyph[g].r, 
				    gr->glyph[g].c, screen, c * 24, r * 35);
			}
		    }
		}
		SDL_Flip( screen);
		usleep( 200000L);

	        SDL_FillRect( screen, NULL, 0);
		for (r=0; r<12; r++) {
		    for (c=0; c<10; c++) {
		        blit24x35( dr1_dung1, 0, 0, screen, c * 24, r * 35);
			blit24x35( img, r, c*2+1, screen, c * 24, r * 35);
		    }
		}
		SDL_Flip( screen);
		usleep( 200000L);
		while ( SDL_PollEvent(&event) ) {
		    switch (event.type) {
		        case SDL_KEYDOWN:
			    switch ( event.key.keysym.sym) {
				case SDLK_1:
				    img = dr1_npcs1;
				    break;
				case SDLK_2:
				    img = dr1_npcs2;
				    break;
				case SDLK_3:
				    img = dr1_npcs3;
				    break;
				case SDLK_d:
				    img = dr1_dung1;
				    break;
				case SDLK_q:
				    exit(3);
			    }
			    break;
			case SDL_MOUSEMOTION:
			    printf("Mouse moved by %d,%d to (%d,%d)\n", 
				   event.motion.xrel, event.motion.yrel,
				   event.motion.x, event.motion.y);
			    break;
			case SDL_MOUSEBUTTONDOWN:
			    printf("Mouse button %d pressed at (%d,%d)\n",
				   event.button.button, event.button.x, event.button.y);
			    exit(1);
			    break;
			case SDL_QUIT:
			    exit(0);
		    }
		}
	    }


	}
    }

    printf("exiting");
    fflush(stdout);
    gets(buf);

}
