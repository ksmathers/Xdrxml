#include <stdio.h>
#include <assert.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "lib/map.h"
#include "lib/glyphset.h"
#include "glyphset.h"
#include "util.h"
#include "text.h"

enum { NOLIGHT, TORCHLIGHT, LANTERNLIGHT };
#define AMBIENTLIGHTDIST 10
int darkdist[] = { 0, 1, 3 };

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
}

typedef struct dr1Point dr1Point;
struct dr1Point {
    int x;
    int y;
};

#define DEFAULT_PTSIZE	18
int setInfo( dr1Text *buf, char *string, int ptsize, int x, int y, enum Position pos) 
{
    static int init = 0;
    static TTF_Font *font;
    static SDL_Surface *temp;
    static SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
    static SDL_Color black = { 0, 0, 0, 0 };
    if (ptsize <= 0) ptsize = DEFAULT_PTSIZE;

    /* Open the font file with the requested point size */
    /* font = TTF_OpenFont("../font/Wizard__.ttf", ptsize); /**/
    /* font = TTF_OpenFont("../font/American.ttf", ptsize); /**/
    font = TTF_OpenFont("../font/MORPHEUS.TTF", ptsize);
    if (!font) {
	    fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
				    ptsize, "Wizard__.ttf", SDL_GetError());
	    exit(2);
    }
    assert(font);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

    /* Render the text */
    buf->textbuf = TTF_RenderText_Shaded(font, string, black, white);
    assert(buf->textbuf);
    switch (pos) {
	case ANCHOR_CTRLEFT:
	case ANCHOR_TOPLEFT:
	case ANCHOR_BOTLEFT:
	    buf->dstrect.x = x;
	    break;
	case ANCHOR_CENTER:
	case ANCHOR_TOPCENTER:
	case ANCHOR_BOTCENTER:
	    buf->dstrect.x = x - buf->textbuf->w/2;
	    break;
	case ANCHOR_CTRRIGHT:
	case ANCHOR_TOPRIGHT:
	case ANCHOR_BOTRIGHT:
	    buf->dstrect.x = x - buf->textbuf->w;
	    break;
    }
    switch (pos) {
	case ANCHOR_CTRLEFT:
	case ANCHOR_CENTER:
	case ANCHOR_CTRRIGHT:
	    buf->dstrect.y = y - buf->textbuf->h / 2;
	    break;
	
	case ANCHOR_TOPLEFT:
	case ANCHOR_TOPCENTER:
	case ANCHOR_TOPRIGHT:
	    buf->dstrect.y = y;
	    break;

	case ANCHOR_BOTLEFT:
	case ANCHOR_BOTCENTER:
	case ANCHOR_BOTRIGHT:
	    buf->dstrect.y = y - buf->textbuf->h;
	    break;
    }
    buf->dstrect.w = buf->textbuf->w;
    buf->dstrect.h = buf->textbuf->h;
    
    printf("Font is generally %d big, and string is %hd big\n",
	    TTF_FontHeight(font), buf->textbuf->h);

    /* Set the text colorkey and convert to display format */
    if ( SDL_SetColorKey(buf->textbuf, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0) < 0 ) {
	fprintf(stderr, "Warning: Couldn't set text colorkey: %s\n",
		SDL_GetError());
    }
    temp = SDL_DisplayFormat(buf->textbuf);
    if ( temp != NULL ) {
	    SDL_FreeSurface(buf->textbuf);
	    buf->textbuf = temp;
    }
    TTF_CloseFont(font);
}

int showInfo( SDL_Surface *screen, dr1Text *txt) {
    SDL_BlitSurface(txt->textbuf, NULL, screen, &txt->dstrect);
}

int main( int argc, char **argv) {
    char buf[80];
    int xpos = 0;
    int ypos = 0;
    int oxpos, oypos;
    dr1Text dr1__name;
    dr1Text dr1__class;
    dr1Text _hits;
    dr1Text _attr[6];
    dr1Text _attrval[6];
    char *msgattr[6] = { "Str:", "Int:", "Wis:", "Dex:", "Con:", "Cha:" };
    dr1Text _scrollmsg[4];
    char *mapfile;
    int i;
     
    if (argc != 2) { 
         printf("Usage: ./test <map file>\n");
	 exit (1);
    }
    mapfile = argv[1];
     
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


    { SDL_Surface *screen;
        dr1Map *map;

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

        dr1GlyphSet_init( screen);
	dr1Text_init( screen);
	dr1_npcs1 = dr1GlyphSet_find( "db-npcs-1");
	loadBorder( screen);

        map = dr1Map_readmap( mapfile);
	xpos = map->startx;
	ypos = map->starty;

	printf("Map loaded\n");

	{
	    SDL_Event event;

	    for (;;) {
	        printf("."); fflush(stdout);
	        SDL_FillRect( screen, NULL, 0);
		showMap( screen, map, xpos, ypos);
		showBorder( screen);
		dr1Text_show( screen);
		SDL_Flip( screen);
		usleep( 100000L);
		while ( SDL_PollEvent(&event) ) {
		    switch (event.type) {
		        case SDL_KEYDOWN:
			    oxpos = xpos;
			    oypos = ypos;
			    switch ( event.key.keysym.sym) {
			        case SDLK_s:
				    printf("screenshot");
				    SDL_SaveBMP(screen, "screenshot.bmp");
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
