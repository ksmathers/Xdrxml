#include "glyph.h"
#include "set.h"
#include "xdrxml.h"

/*-------------------------------------------------------------------
 * dr1GlyphSet
 *
 *    Represents the set of loaded glyphs
 */
typedef struct dr1GlyphSet dr1GlyphSet;
struct dr1GlyphSet {
    dr1Set glyphset;
};

static int cmp( void *a, void *b) {
    dr1Glyph *ag, *bg;
    ag = (dr1Glyph*)a;
    bg = (dr1Glyph*)b;
    if (ag->code < bg->code) return -1;
    if (ag->code > bg->code) return 1;
    return 0;
}
static dr1GlyphSet dr1glyphset =
    {
        { (xdrxmlproc_t)xdr_dr1Glyph, cmp, sizeof(struct dr1Glyph), 0, 0, NULL }
    };


/* Functions */
dr1Glyph* dr1Glyph_find( int code) {
    dr1Glyph b, *r;
    b.code = code;
    r = (dr1Glyph*)dr1Set_find( &dr1glyphset.glyphset, &b);
    return r;
}

void dr1Glyph_add( dr1Glyph *g) {
    dr1Set_add( &dr1glyphset.glyphset, g);
}

void dr1Glyph_loadfile( char *fname) {
    XDR xdrs;
    bool_t res;
    xdr_xml_create( &xdrs, fname, XDR_DECODE);
    if ( !xdr_dr1GlyphSet( &xdrs, "glyphset")) {
	perror("Error reading glyphset");
	exit(1);
    }
    xdrxml_destroy( &xdrs);
}


bool_t 
xdr_dr1GlyphSet( XDR *xdrs, char *node, dr1GlyphSet *gset) {
    return xdr_dr1Set( xdrs, node, &gset->glyphset);
}

/*-------------------------------------------------------------------
 * xdr_dr1Glyph
 */

bool_t
xdr_dr1Glyph( XDR *xdrs, char *node, dr1Glyph *glyph) {
    bool_t flag;
    bool_t res;
    char *fname = NULL;

    xdrxml_group( xdrs, node);
    /* src */
    if (xdrs->x_op == XDR_ENCODE) {
	fname = dr1GlyphTable_file( glyph->src);
    }
    if (!xdrxml_wrapstring( xdrs, "src", &fname)) return FALSE;
    if (xdrs->x_op == XDR_DECODE) {
	glyph->src = dr1GlyphLoader_find( fname);
	free(fname);
    }

    /* row */
    if (!xdrxml_int( xdrs, "r", &glyph->r)) return FALSE;

    /* col */
    if (!xdrxml_int( xdrs, "c", &glyph->c)) return FALSE;

    /* flags */
    /* anim */
    if (!xdrxml_bitfield( xdrs, "anim", glyph->anim, flag, res)) return FALSE;
    if (!xdrxml_bitfield( xdrs, "wall", glyph->wall, flag, res)) return FALSE;
    if (!xdrxml_bitfield( xdrs, "opaque", glyph->opaque, flag, res)) return FALSE;
    if (!xdrxml_bitfield( xdrs, "door", glyph->door, flag, res)) return FALSE;
    if (!xdrxml_bitfield( xdrs, "startinvisible", glyph->startinvisible, flag, res)) return FALSE;

    xdrxml_endgroup( xdrs);
    return TRUE;
}

