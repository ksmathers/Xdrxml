#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/param.h>
#include "glyphset.h"

dr1GlyphSet dr1glyphset;

dr1GlyphTable *dr1GlyphSet_find( char *file) {
    int i;
    for (i=0; i<dr1glyphset.ntables; i++) {
	if (!strcasecmp(dr1glyphset.table[i].fname, file)) {
	    return &dr1glyphset.table[i];
	}
    }
    return NULL;
}

void dr1GlyphSet_init( SDL_Surface *screen) 
{
    int i;
    static char *files[] = {
	"24x35/db-cloud-terrain.bmp",
	"24x35/db-indoor-castle-1.bmp",
	"24x35/db-indoor-castle-2.bmp",
	"24x35/db-indoor-dungeon-1.bmp",
	"24x35/db-indoor-dungeon-2.bmp",
	"24x35/db-indoor-dungeon-3.bmp",
	"24x35/db-indoor-dungeon-4.bmp",
	"24x35/db-mountain-terrain.bmp",
	"24x35/db-npcs-1.bmp",
	"24x35/db-npcs-2.bmp",
	"24x35/db-npcs-3.bmp",
	"24x35/db-objects-1.bmp",
	"24x35/db-objects-2.bmp",
	"24x35/db-objects-3.bmp",
	"24x35/db-outdoor-terrain-1.bmp",
	"24x35/db-outdoor-terrain-2.bmp",
	"24x35/db-snow-icons.bmp",
	"24x35/db-system-icons.bmp",
	"24x35/db-water-icons.bmp"
    };

    dr1glyphset.ntables = sizeof(files)/sizeof(char *);
    dr1glyphset.table = calloc( dr1glyphset.ntables,
	    sizeof( dr1GlyphTable));

    for (i=0; i< dr1glyphset.ntables; i++) {
        int len;
	char *cpos;
	cpos = rindex(files[i], '/');
	if (cpos) {
	    cpos++;
	} else {
	    cpos = files[i];
	}

	len = rindex(files[i], '.') - cpos;
	dr1glyphset.table[i].fname = malloc( len+1);
	strncpy( dr1glyphset.table[i].fname, cpos, len);
	dr1glyphset.table[i].fname[len] = 0;
	dr1glyphset.table[i].size = SIZE24x35;
	dr1glyphset.table[i].image = LoadBMP( files[i], screen);
    }
}

SDL_Surface *LoadBMP(char *file, SDL_Surface *screen)
{
    SDL_Surface *image;
    SDL_Surface *display_image;
    SDL_Rect dest;
    char pathname[MAXPATHLEN];
    SDL_PixelFormat *fmt = screen->format;
    Uint32 key = SDL_MapRGB( fmt, 255, 0, 255);

    /* Load the BMP file into a surface */
    strcpy(pathname, "../gfx/");
    strncat(pathname, file, MAXPATHLEN);
    image = SDL_LoadBMP(pathname);
    if ( image == NULL ) {
	fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
	return;
    }
    display_image = SDL_DisplayFormat( image);
    SDL_SetColorKey( display_image, SDL_SRCCOLORKEY, key);
    return display_image;
}

static void _blit_24x35(
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
}

void dr1GlyphTable_blit(
    dr1GlyphTable *table,
    int row, int col,
    SDL_Surface *screen,
    int x, int y)
{
    assert(table->size == SIZE24x35);
    _blit_24x35( table->image, row, col, screen, x, y);
}

