#ifndef __DR1GLYPHSET__H
#define __DR1GLYPHSET__H
#include <SDL.h>
#include "lib/glyphset.h"

/*-------------------------------------------------------------------
 * dr1GlyphTable
 *
 *    The structure ...
 */

enum dr1GlyphSize { SIZE24x35 };
struct dr1GlyphTable {
    char *fname;
    enum dr1GlyphSize size;
    SDL_Surface *image;
};

/*-------------------------------------------------------------------
 * dr1GlyphSet
 *
 *    The structure ...
 */
struct dr1GlyphSet {
    int ntables;
    dr1GlyphTable *table;
};


/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    Pointer to the glyph table loaded from the specified filename
 *    NULL on error
 *
 *  SIDE EFFECTS:
 */
dr1GlyphTable *dr1GlyphSet_find( char *file);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
void dr1GlyphSet_init( SDL_Surface *screen);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
SDL_Surface *LoadBMP(char *file, SDL_Surface *screen);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
void dr1GlyphTable_blit(
    dr1GlyphTable *table,
    int row, int col,
    SDL_Surface *screen,
    int x, int y);

#endif /* __DR1GLYPHSET__H */
