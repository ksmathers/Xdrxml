#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "xdrxml.h"

#define XSIZE 800
#define YSIZE 600
#define MAPCOLS (XSIZE/24)
#define MAPROWS (YSIZE/35)

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
 * cmpmob()
 */
static int cmpmob( void *ta, void *tb) {
    dr1Mobile *a, *b;
    a = (dr1Mobile*)ta;
    b = (dr1Mobile*)tb;
    if (a->x < b->x) return -1;
    if (a->x > b->x) return 1;
    if (a->y < b->y) return -1;
    if (a->y > b->y) return 1;
    return 0;
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
    dr1Glyph glyphdata;

    if (!fp) {
        printf("Error reading map '%s'\n", fname);
	return NULL;
    }

    map = calloc( 1, sizeof(dr1Map));
    dr1Set_create( &map->moblayer.mobset, cmpmob, NULL);
    map->mapname=strdup(fname);

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
	glyphdata.src = dr1GlyphLoader_find( file);
	if (!glyphdata.src) {
	    printf("Invalid source '%s' on line %d\n", file, line);
	}

	/* get row and col */
	sscanf(cpos, "%d %d%n", &row, &col, &i);
	glyphdata.r = row;
	glyphdata.c = col;
	cpos += i;

	/* get flags */
	while (*cpos && *cpos != '#') {
	    switch (*cpos) {
		case 'd':
		    glyphdata.door = 1;
		    break;
		case 'v':
		    glyphdata.startinvisible = 1;
		    break;
		case 'a':
		    glyphdata.anim = 1;
		    break;
		case 'w':
		    glyphdata.wall = 1;
		    break;
		case 'o':
		    glyphdata.opaque = 1;
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
	} /* while *cpos */

	/* Add the glyph */
	map->graphics[ngraph].nglyphs = glyph + 1;
	map->graphics[ngraph].glyph = realloc(
	    map->graphics[ngraph].glyph,
	    sizeof( dr1Glyph*) * (glyph + 1));
	map->graphics[ngraph].glyph[nglyphs-1] = dr1GlyphSet_add( &glyphdata);


/*        printf("graphics[%d].glyph[%d] = { code %d file %s (%p) row %d cold %d }\n", 
	    ngraph, glyph, code, file, 
	    map->graphics[ngraph].glyph[glyph]->src,
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

#if 0
/*-------------------------------------------------------------------
 * dr1Map_createView
 *
 *  PARAMETERS:
 *     dr1Map map
 *     int x, y
 *     int infravision
 *
 *  RETURNS:
 *     dr1Map
 *
 *  SIDE EFFECTS:
 */
dr1Map*
dr1Map_createView( dr1Map *map, int xpos, int ypos, int light, int infravision) 
{
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
	        /* FIXME: add graphic to view */
	    }

	    if (c==xpos && r==ypos) {
	        /* FIXME: add mobile to view */
	    }
	} /* for c */
    } /* for r */
    return 0;
}
}
#endif

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
xdr_dr1MapGraphic( XDR *xdrs, char *node, dr1MapGraphic *gr) {
    bool_t flag;
    int i;

    xdrxml_group( xdrs, node);
    /* code */
    if (!xdrxml_int32_t( xdrs, "code", &gr->code)) return FALSE;
    
    /* glyph array */
    if (xdrs->x_op == XDR_DECODE) {
        gr->nglyphs = xdrxml_count_children( xdrs, "glyph");
	gr->glyph = calloc( gr->nglyphs, sizeof(dr1Glyph));
    }
    for (i=0; i<gr->nglyphs; i++) {
	if (!xdr_dr1Glyph( xdrs, "glyph", &gr->glyph[i])) return FALSE;
    }
    if (xdrs->x_op == XDR_FREE) {
	free( gr->glyph);
    }
    
    /* start */
    flag = gr->start;
    if (!xdrxml_bool( xdrs, "start", &flag)) return FALSE;
    gr->start = flag;

    /* light */
    flag = gr->light;
    if (!xdrxml_bool( xdrs, "light", &flag)) return FALSE;
    gr->light = flag;

    /* dark */
    flag = gr->dark;
    if (!xdrxml_bool( xdrs, "dark", &flag)) return FALSE;
    gr->dark = flag;
    xdrxml_endgroup( xdrs);

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
xdr_dr1MapSquare( XDR *xdrs, char *node, dr1MapSquare *square, dr1Map *map) {
    int code;
    bool_t flag;
    xdrxml_group( xdrs, node);
    /* graphic */
    if (xdrs->x_op == XDR_ENCODE) {
        if (square->graphic) {
	    code = square->graphic->code;
	} else {
	    code = 0;
	}
    }

    if (!xdrxml_int32_t( xdrs, "code", &code)) return FALSE;
    if (xdrs->x_op == XDR_DECODE) {
        if (code == 0) {
	    square->graphic = NULL;
	} else {
	    square->graphic = findgraphic( map, code);
	}
    }

    /* seen */
    flag = square->seen;
    if (!xdrxml_bool( xdrs, "seen", &flag)) return FALSE;
    square->seen = flag;

    /* invisible */
    flag = square->invisible;
    if (!xdrxml_bool( xdrs, "invisible", &flag)) return FALSE;
    square->invisible = flag;

    /* open */
    flag = square->open;
    if (!xdrxml_bool( xdrs, "open", &flag)) return FALSE;
    square->open = flag;
    xdrxml_endgroup( xdrs);

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
xdr_dr1Map( XDR *xdrs, char *node, dr1Map *map) {
    int i;
    int x, y;
    bool_t flag;

    xdrxml_group( xdrs, node);

    /* map name */
    if (!xdrxml_wrapstring( xdrs, "mapname", &map->mapname)) return FALSE;

    /* graphics */
    if (xdrs->x_op == XDR_DECODE) {
        map->ngraphics = xdrxml_count_children( xdrs, "graphic");
	map->graphics = calloc(map->ngraphics, sizeof(dr1MapGraphic));
    }

    for (i=0; i<map->ngraphics; i++) {
	if ( !xdr_dr1MapGraphic( xdrs, "graphic", &map->graphics[i])) {
	    return FALSE;
	}
    }

    if (xdrs->x_op == XDR_FREE) {
	free(map->graphics);
    }

    /* grid */

    xdrxml_group( xdrs, "grid");
    
    if (xdrs->x_op == XDR_DECODE) {
        xmlNodePtr n = XDRXML_DATA(xdrs)->cur;
	map->ysize = xdrxml_count_children( n, "y");
	printf("ysize %d\n", map->ysize);
    }

    for (y=0; y<map->ysize; y++) {
        xdrxml_group( xdrs, "y");
	
	if (!map->grid && xdrs->x_op == XDR_DECODE) {
	    map->xsize = xdrxml_count_children( n, "x");
	    printf("xsize %d\n", map->xsize);
	    map->grid = calloc( map->xsize * map->ysize, sizeof(dr1MapSquare));
	}

	for (x=0; x<map->xsize; x++) {
	    if (!xdr_dr1MapSquare( xdrs, "x", &map->grid[y * map->xsize + x], map)) 
	    {
		return FALSE;
	    }
	}
	xdrxml_endgroup( xdrs);
    }
    xdrxml_endgroup( xdrs);

    if (xdrs->x_op == XDR_FREE) {
	free( map->grid);
    }

    if (!xdrxml_int( xdrs, "starty", &map->starty)) return FALSE;
    if (!xdrxml_int( xdrs, "startx", &map->startx)) return FALSE;

    /* outdoors */
    flag = map->outdoors;
    if (!xdrxml_bool( xdrs, "outdoors", &flag)) return FALSE;
    map->outdoors = flag;

    /* town */
    flag = map->town;
    if (!xdrxml_bool( xdrs, "town", &flag)) return FALSE;
    map->town = flag;

    xdrxml_endgroup( xdrs);

    if (xdrs->x_op == XDR_DECODE) {
	/* initialize an empty moblayer */
	dr1Set_create( &map->moblayer.mobset, cmpmob, NULL);
    } else if (xdrs->x_op == XDR_FREE) {
	dr1Set_destroy( &map->moblayer.mobset);
    }

    return TRUE;
}


dr1Mobile* dr1Map_findMobile( dr1Map *map, int x, int y) {
    dr1Mobile mtmp, *r;
    mtmp.x=x; 
    mtmp.y=y;
    r = dr1Set_find( map->moblayer.mobset, &mtmp);
    return r;
}

void
dr1Map_animateMobs( dr1Map *map) {
    dr1Mobile **mob = (dr1Mobile**)map->moblayer.mobset.items;
    int i;
    for (i = 0; i < map->moblayer.mobset.len; i++) {
	if (mob[ i]->ai) {
	    switch( mob[ i]->mobtype) {
		case DR1MOB_MONSTER:
/*		    dr1Monster_ai( mob[ i]->mobile.monster); /* FIXME */
		    break;
		default:
		    printf("No AI action for mobtype %d\n", mob[ i]->mobtype);
	    } /* switch */
	} /* if */
    } /* for */
}

void
dr1Map_addMobile( dr1Map *map, dr1Mobile *mob) {
    dr1Mobile **mobs;
    int i=map->moblayer.nmobs;
    int newmax;
    i++;
    mobs=map->moblayer.mobs;
    if (i >= map->moblayer.maxmobs) {
        newmax = map->moblayer.maxmobs + 20;
	mobs = realloc( map->moblayer.mobs, sizeof(dr1Mobile**) * newmax);
	assert(mobs);
	map->moblayer.mobs = mobs;
	map->moblayer.maxmobs = newmax;
    } /* if */
    mobs[i]=mob;
    map->moblayer.nmobs = i;
} /* dr1Map_addMobile */

void 
dr1Map_moveMobile( dr1Map *map, dr1Mobile *mob, int x, int y) {
    mob->location.x = x;
    mob->location.y = y;
}

void
dr1Map_sendMobile( dr1Stream *ios, dr1Mobile *mob) {
    dr1Location *loc;
    loc = mob->location; 
    psendMessage( ios, DR1MSG_175, loc->map, loc->x, loc->y);
}

#if 0
void
dr1Map_sendMobileLayer( dr1Stream *ios, dr1Map *map) {
    for (i=0; i<map->moblayer.nmobs; i++) {
	dr1Map_sendMobile( ios, map->moblayer.mobs[i]);
    }
}
#endif

