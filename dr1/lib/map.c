#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "xdrxml.h"

/*-------------------------------------------------------------------
 * findgraphic()
 */
static dr1MapGraphic* findgraphic( dr1Map *map, int code) {
    int i;
    for (i=0; i<map->ngraphics; i++) {
	if (map->graphics[i].code == code) return &map->graphics[i];
    }
    return NULL;
}

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

dr1Map *dr1Map_readmap( char *fname) {
    FILE *fp = fopen(fname, "r");
    char buf[500];
    int code;
    int i;
    int line;
    char *cpos;
    char file[80];
    int row;
    int col;
    int sep=0;
    int ngraph = 0;
    int xsize = 0;
    int glyph = 0;
    dr1Map *map;
    dr1MapGraphic *g;

    if (!fp) {
        printf("Error reading map '%s'\n", fname);
	return NULL;
    }

    map = calloc( 1, sizeof(dr1Map));

    /* Size of the arrays */
    line = 0;
    do {
	if (!fgets( buf, sizeof(buf), fp)) break;
	line++;
	i = strlen(buf);
	if (buf[i-1] == '\n') buf[--i] = 0;
	if (!sep) {
	    if (buf[0] == '\'') ngraph++;
	    if ( buf[0]==0) {
	        printf("%d graphics\n", ngraph);
		map->ngraphics = ngraph;
		map->graphics = calloc( map->ngraphics, sizeof(dr1MapGraphic));
		sep = line;
	    }
	} else {
	    if (i > xsize) xsize = i;
	}
    } while (!feof(fp));
    line++;
    map->xsize = xsize / 2;
    map->ysize = line - sep - 1;
    printf("size %d x %d\n", map->xsize, map->ysize);
    map->grid = calloc( map->xsize * map->ysize,
		       sizeof(dr1MapSquare) );

    /* Second pass */
    rewind(fp);

    /* Get the MapGraphic array */
    line=0; ngraph=-1;
    do {
	fgets( buf, sizeof(buf), fp); line++;
	i = strlen(buf);
	if (buf[i-1] == '\n') buf[i-1] = 0;
	cpos = buf;

	if ( buf[0]==0) break;

	if ( buf[0]==' ') {
	    glyph++;
	} else {
            glyph=0; ngraph++;
	    if (buf[0] == '\'' && buf[3] == '\'') {
		code = (buf[1]<<8) + buf[2];
		map->graphics[ngraph].code = code;
	    } else {
		printf("Invalid input line %d\n", line);
	    }
	    cpos += 4;
	}
	
	/* get filename */
	cpos += strspn( cpos, " \t");
        i = strcspn( cpos, " \t");
	memcpy( file, cpos, i);
	file[i] = 0;
	cpos += i;
	map->graphics[ngraph].nglyphs = glyph + 1;
	map->graphics[ngraph].glyph = realloc(
	    map->graphics[ngraph].glyph,
	    sizeof( dr1Glyph) * (glyph + 1));
	bzero( &map->graphics[ngraph].glyph[glyph], sizeof(dr1Glyph));
	map->graphics[ngraph].glyph[glyph].src = dr1GlyphSet_find( file);
	if (!map->graphics[ngraph].glyph[glyph].src) {
	    printf("Invalid source '%s' on line %d\n", file, line);
	}

	/* get row and col */
	sscanf(cpos, "%d %d%n", &row, &col, &i);
	map->graphics[ngraph].glyph[glyph].r = row;
	map->graphics[ngraph].glyph[glyph].c = col;
	cpos += i;

	/* get flags */
	while (*cpos && *cpos != '#') {
	    switch (*cpos) {
		case 'd':
		    map->graphics[ngraph].glyph[glyph].door = 1;
		    break;
		case 'v':
		    map->graphics[ngraph].glyph[glyph].startinvisible = 1;
		    break;
		case 'a':
		    map->graphics[ngraph].glyph[glyph].anim = 1;
		    break;
		case 'w':
		    map->graphics[ngraph].glyph[glyph].wall = 1;
		    break;
		case 'o':
		    map->graphics[ngraph].glyph[glyph].opaque = 1;
		    break;
		case 's':
		    map->graphics[ngraph].start = 1;
		    break;
		case 'D':
		    map->graphics[ngraph].dark = 1;
		    break;
		case 'O':
		    map->outdoors = 1;
		case 'l':
		    map->graphics[ngraph].light = 1;
		    break;
	    }
	    cpos++;
	}

/*        printf("graphics[%d].glyph[%d] = { code %d file %s (%p) row %d cold %d }\n", 
	    ngraph, glyph, code, file, 
	    map->graphics[ngraph].glyph[glyph].src,
	    row, col); /**/
    } while (!feof(fp) && *buf != 0);

    /* Get map data */
    sep=line;
    row=0;
    do {
	col=0; 
	if (!fgets( buf, sizeof(buf), fp)) break; 
	line++;
	i = strlen(buf);
	if (buf[i-1] == '\n') buf[i-1] = 0;
	
	for( cpos=buf; *cpos; cpos+=2) {
	    if (!cpos[1]) {
		printf("Odd line length at line %d\n", line);
		break;
	    }
	    code = (cpos[0]<<8) + cpos[1];
	    
	    g = findgraphic( map, code);
	    if (!g) {
		printf( "Unknown graphic '%c%c' (%d) on line %d\n", 
			cpos[0], cpos[1], code, line);
	    }
	    if (g->start) {
		map->starty = row;
		map->startx = col;
	    }
	    if (g->glyph[0].startinvisible) {
		map->grid[ row*map->xsize + col].invisible = 1;
	    }
	    map->grid[ row*map->xsize + col].graphic = g;
	    col++;
	}
	row++;
    } while (!feof(fp));

    return map;

}

int dr1Map_setgraphic( dr1Map *map, char c[2], int x, int y) {
    dr1MapGraphic *g = findgraphic( map, (c[0]<<8) + c[1]);
    if (!g) return 1;
    map->grid[ y*map->xsize + x].graphic = g;
    return 0;
}


/*-------------------------------------------------------------------
 * xdr_dr1Glyph
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

bool_t
xdr_dr1Glyph( XDR *xdrs, dr1Glyph *glyph) {
    bool_t flag;
    char *fname = NULL;

    /* src */
    if (xdrs->x_op == XDR_ENCODE) {
	fname = dr1GlyphTable_file( glyph->src);
    }
    xdr_attr( xdrs, "src");
    if (!xdrxml_wrapstring( xdrs, &fname)) return FALSE;
    if (xdrs->x_op == XDR_DECODE) {
	glyph->src = dr1GlyphSet_find( fname);
	free(fname);
    }

    /* row */
    xdr_attr( xdrs, "r");
    if (!xdr_int( xdrs, &glyph->r)) return FALSE;

    /* col */
    xdr_attr( xdrs, "c");
    if (!xdr_int( xdrs, &glyph->c)) return FALSE;

    /* flags */
    /* anim */
    xdr_attr( xdrs, "anim");
    flag = glyph->anim;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    glyph->anim = flag;

    xdr_attr( xdrs, "wall");
    flag = glyph->wall;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    glyph->wall = flag;

    xdr_attr( xdrs, "opaque");
    flag = glyph->opaque;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    glyph->opaque = flag;

    xdr_attr( xdrs, "door");
    flag = glyph->door;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    glyph->door = flag;

    xdr_attr( xdrs, "startinvisible");
    flag = glyph->startinvisible;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    glyph->startinvisible = flag;

    return TRUE;
}


/*-------------------------------------------------------------------
 * xdr_dr1MapGraphic
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
bool_t
xdr_dr1MapGraphic( XDR *xdrs, dr1MapGraphic *gr) {
    bool_t flag;
    int i;

    /* code */
    xdr_attr( xdrs, "code");
    if (!xdr_int( xdrs, &gr->code)) return FALSE;
    
    /* nglyphs */
    xdr_attr( xdrs, "nglyphs");
    if (!xdr_int( xdrs, &gr->nglyphs)) return FALSE;

    /* glyph array */
    if (xdrs->x_op == XDR_DECODE) {
	gr->glyph = calloc( gr->nglyphs, sizeof(dr1Glyph));
    }
    for (i=0; i<gr->nglyphs; i++) {
	xdr_push_note( xdrs, "glyph");
	if (!xdr_dr1Glyph( xdrs, &gr->glyph[i])) return FALSE;
	xdr_pop_note( xdrs);
    }
    if (xdrs->x_op == XDR_FREE) {
	free( gr->glyph);
    }
    
    /* start */
    xdr_attr( xdrs, "start");
    flag = gr->start;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    gr->start = flag;

    /* light */
    xdr_attr( xdrs, "light");
    flag = gr->light;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    gr->light = flag;

    /* dark */
    xdr_attr( xdrs, "dark");
    flag = gr->dark;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    gr->dark = flag;

    return TRUE;
}

/*-------------------------------------------------------------------
 * xdr_dr1MapSquare
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

bool_t
xdr_dr1MapSquare( XDR *xdrs, dr1MapSquare *square, dr1Map *map) {
    int code;
    bool_t flag;
    /* graphic */
    if (xdrs->x_op == XDR_ENCODE) {
        if (square->graphic) {
	    code = square->graphic->code;
	} else {
	    code = 0;
	}
    }

    xdr_attr( xdrs, "code");
    if (!xdr_int( xdrs, &code)) return FALSE;
    if (xdrs->x_op == XDR_DECODE) {
        if (code == 0) {
	    square->graphic = NULL;
	} else {
	    square->graphic = findgraphic( map, code);
	}
    }

    /* seen */
    xdr_attr( xdrs, "seen");
    flag = square->seen;
    if (!xdr_bool( xdrs, &flag)) return FALSE;
    square->seen = flag;

    /* invisible */
    xdr_attr( xdrs, "invisible");
    flag = square->invisible;
    if (!xdrxml_bool( xdrs, &flag)) return FALSE;
    square->invisible = flag;

    /* open */
    xdr_attr( xdrs, "open");
    flag = square->open;
    if (!xdr_bool( xdrs, &flag)) return FALSE;
    square->open = flag;

    return TRUE;
}

/*-------------------------------------------------------------------
 * xdr_dr1Map
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
bool_t
xdr_dr1Map( XDR *xdrs, dr1Map *map) {
    int i;
    int x, y;
    bool_t flag;

    xdr_attr( xdrs, "ngraphics");
    if (!xdr_int( xdrs, &map->ngraphics)) return FALSE;

    /* graphics */
    if (xdrs->x_op == XDR_DECODE) {
	map->graphics = calloc(map->ngraphics, sizeof(dr1MapGraphic));
    }

    for (i=0; i<map->ngraphics; i++) {
	xdr_push_note( xdrs, "graphic");
	if ( !xdr_dr1MapGraphic( xdrs, &map->graphics[i])) {
	    return FALSE;
	}
	xdr_pop_note( xdrs);
    }

    if (xdrs->x_op == XDR_FREE) {
	free(map->graphics);
    }

    xdr_attr( xdrs, "xsize");
    if (!xdr_int( xdrs, &map->xsize)) return FALSE;

    xdr_attr( xdrs, "ysize");
    if (!xdr_int( xdrs, &map->ysize)) return FALSE;

    /* grid */
    if (xdrs->x_op == XDR_DECODE) {
	map->grid = calloc( map->xsize * map->ysize, sizeof(dr1MapSquare));
    }

    xdr_push_note( xdrs, "grid");
    for (y=0; y<map->ysize; y++) {
        xdr_push_note( xdrs, "y");
	for (x=0; x<map->xsize; x++) {
	    xdr_push_note( xdrs, "x");
	    if (!xdr_dr1MapSquare( xdrs, &map->grid[y * map->xsize + x], map)) 
	    {
		return FALSE;
	    }
	    xdr_pop_note( xdrs);
	}
	xdr_pop_note( xdrs);
    }
    xdr_pop_note( xdrs);

    if (xdrs->x_op == XDR_FREE) {
	free( map->grid);
    }

    xdr_attr( xdrs, "starty");
    if (!xdr_int( xdrs, &map->starty)) return FALSE;

    xdr_attr( xdrs, "startx");
    if (!xdr_int( xdrs, &map->startx)) return FALSE;

    /* outdoors */
    xdr_attr( xdrs, "outdoors");
    flag = map->outdoors;
    if (!xdr_bool( xdrs, &flag)) return FALSE;
    map->outdoors = flag;

    /* town */
    xdr_attr( xdrs, "town");
    flag = map->town;
    if (!xdr_bool( xdrs, &flag)) return FALSE;
    map->town = flag;

    return TRUE;
}

