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
SDL_Surface *dr1_objects2;

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
    if (!display_image) return;
     
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

#define XSIZE 800
#define YSIZE 600
#define MAPCOLS (XSIZE/24)
#define MAPROWS (YSIZE/35)

int main( int argc, char **argv) {
    char buf[80];
    int xpos = 0;
    int ypos = 0;
    int oxpos, oypos;
    int alt = 0;
    
    /* Initialize graphics */
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	exit(1);
    }
    atexit(SDL_Quit);

    { SDL_Surface *screen;
        dr1Map map;

#if 1
	screen = SDL_SetVideoMode(XSIZE, YSIZE, 16, SDL_SWSURFACE);
#else
	screen = SDL_SetVideoMode(XSIZE, YSIZE, 16, 
	    SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
#endif
	if ( screen == NULL ) {
	    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
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
	dr1_objects2 = LoadBMP( GFXDIR "/db-objects-2.bmp", screen);

        map = readmap( "map.txt");
	xpos = map.startcol;
	ypos = map.startrow;

	printf("Map loaded\n");

	{
	    SDL_Event event;
	    int r,c,g;
	    SDL_Surface *img = dr1_npcs1;

	    for (;;) {
	        if (alt==0) alt = 1;
		else alt=0;
	        printf("."); fflush(stdout);
	        SDL_FillRect( screen, NULL, 0);
		for (r=ypos-MAPROWS/2; r<map.grid.ysize; r++) {
		    if (r<0) continue;
		    if ((r-ypos) > MAPROWS) break;
		    for (c=xpos-MAPCOLS/2; c<map.grid.xsize; c++) {	    
		        dr1MapGraphic *gr = map.grid.graphic[ r*map.grid.xsize +c];
		        if (c<0) continue;
			if ((c-xpos) > MAPCOLS) break;
			if (!gr) break;
		        for (g=gr->nglyphs-1; g>=0; g--) {
			    blit24x35( gr->glyph[g].src, gr->glyph[g].r, 
				    gr->glyph[g].c + (gr->glyph[g].anim?alt:0), 
				    screen, 
				    (c - xpos + MAPCOLS/2) * 24, 
				    (r - ypos + MAPROWS/2) * 35);
			}

			if (c==xpos && r==ypos) {
			    blit24x35( dr1_npcs1, 0, 1, screen, 
				(c - xpos + MAPCOLS/2) * 24, 
				(r - ypos + MAPROWS/2) * 35 );
			}
		    }
		}
		SDL_Flip( screen);
		usleep( 100000L);
		while ( SDL_PollEvent(&event) ) {
		    switch (event.type) {
		        case SDL_KEYDOWN:
			    oxpos = xpos;
			    oypos = ypos;
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
				case SDLK_h:
				    xpos--;
				    break;
				case SDLK_j:
				    ypos++;
				    break;
				case SDLK_k:
				    ypos--;
				    break;
				case SDLK_l:
				    xpos++;
				    break;
				case SDLK_q:
				    exit(3);
			    }
			    {
				dr1MapGraphic *gr = map.grid.graphic[ ypos*map.grid.xsize + xpos];
				if (gr->glyph[0].wall) {
				    xpos = oxpos;
				    ypos = oypos;
				}
			    }
			    break;
			case SDL_MOUSEMOTION:
/*			    printf("Mouse moved by %d,%d to (%d,%d)\n", 
				   event.motion.xrel, event.motion.yrel,
				   event.motion.x, event.motion.y); /**/
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
