#ifndef __LIB_DR1GLYPHSET__H
#define __LIB_DR1GLYPHSET__H

/*-------------------------------------------------------------------
 * dr1GlyphTable
 *
 *    The structure ...
 */

typedef struct dr1GlyphTable dr1GlyphTable;

/*-------------------------------------------------------------------
 * dr1GlyphSet
 *
 *    The structure ...
 */
typedef struct dr1GlyphSet dr1GlyphSet;

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
 *    Pointer to filename that the glyph table was loaded from.  This is 
 *    the opposite of the dr1GlyphSet_find method listed above.
 *
 *  SIDE EFFECTS:
 */
char *dr1GlyphTable_file( dr1GlyphTable *gtab);

#endif /* __LIB_DR1GLYPHSET__H */

