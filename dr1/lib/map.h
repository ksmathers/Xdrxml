#ifndef __DR1MAP__H
#define __DR1MAP__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "glyphset.h"

/*-------------------------------------------------------------------
 * dr1Glyph
 *
 *    Represents one layer of a mult-layered static map square
 */

typedef struct dr1Glyph dr1Glyph;
struct dr1Glyph {
    dr1GlyphTable *src;
    int r;
    int c;
    int anim:1;			/* animated */
    int wall:1;			/* blocks passage */
    int opaque:1;		/* blocks vision */
    int door:1;			/* openable door ('o' command) */
    int startinvisible:1;       /* initial visibility */
};

/*-------------------------------------------------------------------
 * dr1MapGraphic
 *
 *    Represents a type of map square (e.g: Dungeon floor)
 *
 *    By convention all doorlike icons (icons made visible and 
 *    invisible) should be at the top of the stack.  The MapSquare
 *    visible field controls the top icon visibility.
 */

typedef struct dr1MapGraphic dr1MapGraphic;
struct dr1MapGraphic {
    int code;
    int nglyphs;
    dr1Glyph *glyph; 
    int start:1;
    int light:1;	/* ambient light */
    int dark:1;		/* magically dark */
};


/*-------------------------------------------------------------------
 * dr1MapSquare
 *
 *    Represents a map square.  This includes both a static
 *    part (if you modify this then all squares with the same
 *    graphic will be changed throughout the map), and a 
 *    dynamic part.
 */

typedef struct dr1MapSquare dr1MapSquare;
struct dr1MapSquare {
    /* static */
    dr1MapGraphic *graphic;

    /* dynamic */
    int seen:1;
    int invisible:1;		/* top glyph is invisible */
    int open:1;			/* top glyph is open */
};

/*-------------------------------------------------------------------
 * dr1
 *
 *    This structure holds the entire map.
 *
 *    TODO:
 *      need to add a mobile layer
 */

typedef struct dr1Map dr1Map;
struct dr1Map {
    int ngraphics;
    dr1MapGraphic *graphics;
    int xsize; 
    int ysize;
    dr1MapSquare *grid;		/* [xsize*ysize] */
    int startx;
    int starty;
    int outdoors:1;		/* in the great out of doors */
    int town:1;			/* is a town */
};

/*-------------------------------------------------------------------
 * dr1Map_readmap
 *
 *    The method loads a map from a map definition file.
 *
 *  PARAMETERS:
 *    fname   File to read from
 *
 *  RETURNS:
 *    dr1Map  An initalized map object.  Call dr1Map_free() to
 *            destroy it.
 *
 *  SIDE EFFECTS:
 */

dr1Map* dr1Map_readmap( char *fname);


int dr1Map_setgraphic( dr1Map *map, char c[2], int x, int y);
bool_t xdr_dr1Map( XDR *xdrs, char *node, dr1Map *);
#endif /* __DR1MAP__H */
