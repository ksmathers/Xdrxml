#include <stdio.h>
#include <SDL.h>
#include <assert.h>

#include "map.h"
#include "util.h"
#define NOLIGHTDIST 0
#define TORCHLIGHTDIST 1
#define LANTERNLIGHTDIST 3
#define AMBIENTLIGHTDIST 10

SDL_Surface *dr1_npcs1;
SDL_Surface *dr1_npcs2;
SDL_Surface *dr1_npcs3;
SDL_Surface *dr1_dung1;
SDL_Surface *dr1_dung2;
SDL_Surface *dr1_dung3;
SDL_Surface *dr1_castle1;
SDL_Surface *dr1_objects1;
SDL_Surface *dr1_objects2;

#define TORCH 1
#define LANTERN 2
#define NOLIGHT 0
int lightsource = TORCH;

struct border_t {
    int top;
    int left;
    int right;
    int bottom;
    SDL_Surface *nw;
    SDL_Surface *ne;
    SDL_Surface *se;
    SDL_Surface *sw;
    SDL_Surface *n;
    SDL_Surface *e;
    SDL_Surface *s;
    SDL_Surface *w;
} dr1_border;

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

opendoor( dr1Map *map, int xpos, int ypos) {
    int x,y;
    for (x = xpos-1; x<=xpos+1; x++) {
	for (y= ypos-1; y<=ypos+1; y++) {
	    dr1MapGrid *gr = &map->grid[ y*map->xsize + x];
	    if (!gr->graphic) continue;
	    if (gr->graphic->glyph[0].door) {
		gr->open ^= 1;
		gr->invisible ^= 1;
	    }
	}
    }
}


int loadBorder( SDL_Surface *screen) {
    struct border_t *b = &dr1_border;
    b->n = LoadBMP( GFXDIR "/bdrop1-n.bmp", screen);
    assert(b->n);
    b->e = LoadBMP( GFXDIR "/bdrop1-e.bmp", screen);
    assert(b->e);
    b->s = LoadBMP( GFXDIR "/bdrop1-s.bmp", screen);
    assert(b->s);
    b->w = LoadBMP( GFXDIR "/bdrop1-w.bmp", screen);
    assert(b->w);
    b->nw = LoadBMP( GFXDIR "/bdrop1-nw.bmp", screen);
    assert(b->nw);
    b->sw = LoadBMP( GFXDIR "/bdrop1-sw.bmp", screen);
    assert(b->sw);
    b->ne = LoadBMP( GFXDIR "/bdrop1-ne.bmp", screen);
    assert(b->ne);
    b->se = LoadBMP( GFXDIR "/bdrop1-se.bmp", screen);
    assert(b->se);
    return 0;
}

int showBorder( SDL_Surface *screen) {
    struct border_t *b = &dr1_border;
    SDL_Rect dest;
    int x, x1, x2;
    int y, y1, y2;
    x1 = b->nw->w;
    x2 = screen->w - b->se->w;
    y1 = b->nw->h;
    y2 = screen->h - b->se->h;

    /* nw */
    dest.x = 0; dest.y = 0; dest.w = b->nw->w; dest.h = b->nw->h;
    SDL_BlitSurface(b->nw, NULL, screen, &dest);

    /* ne */
    dest.x = x2; dest.y = 0; dest.w = b->ne->w; dest.h = b->ne->h;
    SDL_BlitSurface(b->ne, NULL, screen, &dest);

    /* sw */
    dest.x = 0; dest.y = y2; dest.w = b->sw->w; dest.h = b->sw->h;
    SDL_BlitSurface(b->sw, NULL, screen, &dest);

    /* se */
    dest.x = x2; dest.y = y2; dest.w = b->se->w; dest.h = b->se->h;
    SDL_BlitSurface(b->se, NULL, screen, &dest);

    /* n/s */
    for (x = x1; x < x2; x += b->n->w) {
        /* n */
	dest.x = x; dest.y = 0; dest.w = b->n->w; dest.h = b->n->h;
	SDL_BlitSurface(b->n, NULL, screen, &dest);

        /* s */
	dest.x = x; dest.y = screen->h - b->s->h; 
	dest.w = b->s->w; dest.h = b->s->h;
	SDL_BlitSurface(b->s, NULL, screen, &dest);
    }

    /* e/w */
    for (y = y1; y < y2; y += b->w->h) {
        /* w */
	dest.x = 0; dest.y = y; dest.w = b->w->w; dest.h = b->w->h;
	SDL_BlitSurface(b->w, NULL, screen, &dest);

        /* e */
	dest.x = screen->w - b->e->w; dest.y = y;
	dest.w = b->e->w; dest.h = b->e->h;
	SDL_BlitSurface(b->e, NULL, screen, &dest);
    }
}

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

	dr1_npcs1 = LoadBMP( GFXDIR "/24x35/db-npcs-1.bmp", screen);
	dr1_npcs2 = LoadBMP( GFXDIR "/24x35/db-npcs-2.bmp", screen);
	dr1_npcs3 = LoadBMP( GFXDIR "/24x35/db-npcs-3.bmp", screen);
	dr1_dung1 = LoadBMP( GFXDIR "/24x35/db-indoor-dungeon-1.bmp", screen);
	dr1_dung2 = LoadBMP( GFXDIR "/24x35/db-indoor-dungeon-2.bmp", screen);
	dr1_dung3 = LoadBMP( GFXDIR "/24x35/db-indoor-dungeon-3.bmp", screen);
	dr1_castle1 = LoadBMP( GFXDIR "/24x35/db-indoor-castle-1.bmp", screen);
	dr1_objects1 = LoadBMP( GFXDIR "/24x35/db-objects-1.bmp", screen);
	dr1_objects2 = LoadBMP( GFXDIR "/24x35/db-objects-2.bmp", screen);
	loadBorder( screen);

        map = readmap( "map.txt");
	xpos = map.startcol;
	ypos = map.startrow;

	printf("Map loaded\n");

	{
	    SDL_Event event;
	    int r,c,g;
	    int ambientlight = 0;
	    SDL_Surface *img = dr1_npcs1;

	    for (;;) {
	        alt ^= 1;
	        printf("."); fflush(stdout);
	        SDL_FillRect( screen, NULL, 0);
		ambientlight = map.grid[ ypos*map.xsize + xpos].graphic->light;
		for (r=ypos-MAPROWS/2; r<map.ysize; r++) {
		    if (r<0) continue;
		    if ((r-ypos) > MAPROWS) break;
		    for (c=xpos-MAPCOLS/2; c<map.xsize; c++) {	    
		        dr1MapGrid *gr = &map.grid[ r*map.xsize +c];
			/* reject if off of the map */
		        if (c<0) continue;
			if ((c-xpos) > MAPCOLS) break;

			/* reject if there is no graphic in this part of map */
			if (!gr->graphic) break;

                        if (!gr->seen) {
			    /* reject if beyond light distance */
			    if (!gr->graphic->light) {
				int dx, dy, d;
				int ld;
				
				dx = xpos - c;
				if (dx < 0) dx = -dx;
				dy = (ypos - r) * 3 / 2;
				if (dy < 0) dy = -dy;
				if (dx>dy) { 
				    d = dx + (dy>>1);
				} else { 
				    d = dy + (dx>>1);
				}

				if (gr->graphic->dark) {
				    /* magically dark square */
				    ld = NOLIGHTDIST;
				} else {
				    if (ambientlight) {
				        /* lit square */
					ld = AMBIENTLIGHTDIST;
				    } else {
				        /* unlit square */
					if (lightsource == TORCH) {
					    /* using torch light */
					    ld = TORCHLIGHTDIST;
					} else if (lightsource == LANTERN) {
					    /* using lantern */
					    ld = LANTERNLIGHTDIST;
					} else {
					    /* using sense of touch */
					    ld = NOLIGHTDIST;
					}
				    }
				}
				if (d>ld) continue;
			    }

			    /* reject if not in view */
			    if (!dr1_los(ypos,xpos, r, c, &map)) continue;
			}
			
			/* else draw all of the glyphs in the graphic */
			gr->seen = 1;
			assert(gr->graphic->nglyphs>0);
		        for (g=gr->graphic->nglyphs-1; g >= (gr->invisible?1:0); g--) {
			    blit24x35( gr->graphic->glyph[g].src, 
			            gr->graphic->glyph[g].r, 
				    gr->graphic->glyph[g].c + 
				      (gr->graphic->glyph[g].anim?alt:0), 
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
		showBorder( screen);
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
				case SDLK_o:
				    opendoor( &map, xpos, ypos);
				    break;
				case SDLK_q:
				    exit(3);
			    }
			    {
				dr1MapGrid *gr = &map.grid[ ypos*map.xsize + xpos];
				if ( !gr->graphic || 
				     gr->graphic->glyph[0].wall ||
				     ( gr->graphic->glyph[0].door && 
				       !gr->open) 
				   )
				{
				    if (gr->graphic) gr->seen = 1;
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
