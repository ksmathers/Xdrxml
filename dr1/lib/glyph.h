#ifndef __GLYPH__H
#define __GLYPH__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

#include "glyphloader.h"

/*-------------------------------------------------------------------
 * dr1Glyph
 *
 *    Represents one layer of a multi-layered static map square
 */

typedef struct dr1Glyph dr1Glyph;
struct dr1Glyph {
    int code;			/* unique code for the glyph */
    dr1GlyphTable *src;
    int r;
    int c;
    int anim:1;			/* animated */
    int wall:1;			/* blocks passage */
    int opaque:1;		/* blocks vision */
    int door:1;			/* openable door ('o' command) */
    int startinvisible:1;       /* initial visibility (e.g: secret door) */
};

/*-------------------------------------------------------------------
 * dr1Glyph_find
 *
 *    The method locates a glyph from the set of defined glyphs
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    The glyph matching the desired code, or NULL if not found.
 *
 *  SIDE EFFECTS:
 */

dr1Glyph* dr1Glyph_find( int code);

/*-------------------------------------------------------------------
 * dr1Glyph_add
 *
 *    The method adds a new glyph to the set of defined glyphs
 *
 *  PARAMETERS:
 *    A glyph to be added.  The input glyph is not kept, so the
 *    caller is responsible for cleaning up the glyph structure
 *    that is passed in.
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    Modifies the global table of defined glyphs.
 */
void dr1Glyph_add( dr1Glyph *g);


/*-------------------------------------------------------------------
 * xdr_dr1Glyph
 *
 *    Serializes the glyph for transmission
 *
 *  PARAMETERS:
 *    xdrs  The stream to serialize to/from
 *    node  The name of the root element for this object
 *    glyph The object to be serialized
 *
 *  RETURNS:
 *    1     Success
 *    0     Failure
 *
 *  SIDE EFFECTS:
 *    Modifies the xdrs stream
 */
bool_t
xdr_dr1Glyph( XDR *xdrs, char *node, dr1Glyph *glyph);
#endif
