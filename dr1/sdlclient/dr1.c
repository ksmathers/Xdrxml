#include <stdio.h>
#include <assert.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "lib/map.h"
#include "lib/glyphset.h"
#include "lib/strbuf.h"
#include "lib/protocol.h"
#include "lib/xdrxml.h"
#include "glyphset.h"
#include "util.h"
#include "text.h"
#include "comm.h"

#include "../player.h"

enum { NOLIGHT, TORCHLIGHT, LANTERNLIGHT };
#define AMBIENTLIGHTDIST 10
int darkdist[] = { 0, 1, 3 };

struct {
    char *server;
    SDL_Surface *screen;
    dr1Player player;
} ctx = { NULL };

dr1GlyphTable *dr1_npcs1;

int lightsource = TORCHLIGHT;

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

#define XSIZE 800
#define YSIZE 600
#define MAPCOLS (XSIZE/24)
#define MAPROWS (YSIZE/35)

void
opendoor( dr1Map *map, int xpos, int ypos) {
    int x,y;
    for (x = xpos-1; x<=xpos+1; x++) {
	for (y= ypos-1; y<=ypos+1; y++) {
	    dr1MapSquare *gr = &map->grid[ y*map->xsize + x];
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
    return 0;
}

int showMap( SDL_Surface* screen, dr1Map *map, int xpos, int ypos) {
    int r,c,g;
    int ambientlight = 0;
    static int alt = 0;
    alt ^= 1;

    ambientlight = map->grid[ ypos*map->xsize + xpos].graphic->light;
    for (r=ypos-MAPROWS/2; r<map->ysize; r++) {
	if (r<0) continue;
	if ((r-ypos) > MAPROWS) break;
	for (c=xpos-MAPCOLS/2; c<map->xsize; c++) {	    
	    dr1MapSquare *gr = &map->grid[ r*map->xsize +c];
	    /* reject if off of the map */
	    if (c<0) continue;
	    if ((c-xpos) > MAPCOLS) break;

	    /* reject if there is no graphic in this part of map */
	    if (!gr->graphic) break;

	    if (!gr->seen) {
		/* reject if beyond light distance */
		if (!map->outdoors && !gr->graphic->light) {
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
			ld = darkdist[ NOLIGHT];
		    } else {
			if (ambientlight) {
			    /* lit area */
			    ld = AMBIENTLIGHTDIST;
			} else {
			    /* unlit square */
			    ld = darkdist[ lightsource];
			}
		    }
		    if (d>ld) continue;
		}

		/* reject if not in view */
		if (!dr1_los(ypos,xpos, r, c, map)) continue;
	    }
	    
	    /* else draw all of the glyphs in the graphic */
	    gr->seen = 1;
	    assert(gr->graphic->nglyphs>0);
	    for (g=gr->graphic->nglyphs-1; g >= (gr->invisible?1:0); g--) {
		dr1GlyphTable_blit( gr->graphic->glyph[g].src, 
			gr->graphic->glyph[g].r, 
			gr->graphic->glyph[g].c + 
			  (gr->graphic->glyph[g].anim?alt:0), 
			screen, 
			(c - xpos + MAPCOLS/2) * 24, 
			(r - ypos + MAPROWS/2) * 35);
	    }

	    if (c==xpos && r==ypos) {
		dr1GlyphTable_blit( dr1_npcs1, 0, 1, screen, 
		    (c - xpos + MAPCOLS/2) * 24, 
		    (r - ypos + MAPROWS/2) * 35 );
	    }
	} /* for c */
    } /* for r */
    return 0;
}

typedef struct dr1Point dr1Point;
struct dr1Point {
    int x;
    int y;
};

#ifndef NDEBUG
extern int EF_ALLOW_MALLOC_0;
#endif

int main( int argc, char **argv) {
    char buf[80];
    int xpos = 0;
    int ypos = 0;
    int oxpos, oypos;
    char *server;

    EF_ALLOW_MALLOC_0 = 1;
     
    if (argc != 2) { 
         printf("Usage: ./dr1 <server>\n");
	 exit (1);
    }
    server = argv[1];
     
    /* Initialize graphics */
    if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 ) {
	fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
	exit(1);
    }
    atexit(SDL_Quit);

    /* Initialize the TTF library */
    if ( TTF_Init() < 0 ) {
	fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
	exit(2);
    }
    atexit(TTF_Quit);

    /* initialize globals */
    dr1Dice_seed();

    {   
        dr1Map *map = NULL;

#if 1
	ctx.screen = SDL_SetVideoMode(XSIZE, YSIZE, 16, SDL_SWSURFACE);
#else
	ctx.screen = SDL_SetVideoMode(XSIZE, YSIZE, 16, 
	    SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
#endif
	if ( ctx.screen == NULL ) {
	    fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
	    exit(1);
	}

        /* Load graphics */
        dr1GlyphSet_init( ctx.screen);
	dr1Text_init( ctx.screen);
	dr1_npcs1 = dr1GlyphSet_find( "db-npcs-1");
	loadBorder( ctx.screen);


	dr1Text_infoMessage("Welcome to Dragon's Reach, adventurer!", ctx.screen);

	{
	    SDL_Event event;
	    pthread_t comm;

	    pthread_create( &comm, NULL, comm_main, server);

	    for (;;) {
	        printf("."); fflush(stdout);

		/* Draw the screen */
	        SDL_FillRect( ctx.screen, NULL, 0);
		if (map) showMap( ctx.screen, map, xpos, ypos);
		showBorder( ctx.screen);
		dr1Text_show( ctx.screen);
		SDL_Flip( ctx.screen);

		/* Get input */
		usleep( 100000L); /**/
		while ( SDL_PollEvent(&event) ) {
		    switch (event.type) {
		        case SDL_KEYDOWN:
			    oxpos = xpos;
			    oypos = ypos;
			    switch ( event.key.keysym.sym) {
			        case SDLK_s:
				    printf("screenshot");
				    SDL_SaveBMP(ctx.screen, "screenshot.bmp");
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
				    opendoor( map, xpos, ypos);
				    break;
				case SDLK_q:
				    exit(3);
				default:
			    }
			    {
				dr1MapSquare *gr = &map->grid[ ypos*map->xsize + xpos];
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
#if 0
			    printf("Mouse moved by %d,%d to (%d,%d)\n", 
				   event.motion.xrel, event.motion.yrel,
				   event.motion.x, event.motion.y); 
#endif
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
