#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

dr1MapGraphic* findgraphic( dr1Map *map, int code) {
    int i;
    for (i=0; i<map->ngraphics; i++) {
	if (map->graphics[i].code == code) return &map->graphics[i];
    }
    return NULL;
}

dr1Map readmap( char *fname) {
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
    int ngraph;
    int xsize = 0;
    int glyph = 0;

    dr1Map map;
    dr1MapGraphic *g;


    /* Size of the arrays */
    line = 0;
    do {
	fgets( buf, sizeof(buf), fp); line++;
	i = strlen(buf);
	if (buf[i-1] == '\n') buf[--i] = 0;
	if (!sep) {
	    if (buf[0] == '\'') ngraph++;
	    if ( buf[0]==0) {
	        printf("%d graphics\n", ngraph);
		map.ngraphics = ngraph;
		map.graphics = calloc( map.ngraphics, sizeof(dr1MapGraphic));
		sep = line;
	    }
	} else {
	    if (i > xsize) xsize = i;
	}
    } while (!feof(fp));
    map.grid.xsize = xsize / 2;
    map.grid.ysize = line - sep - 1;
    printf("size %d x %d\n", map.grid.xsize, map.grid.ysize);
    map.grid.graphic = malloc( map.grid.xsize * map.grid.ysize *
			       sizeof(dr1MapGraphic *) );

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
		map.graphics[line-1].code = code;
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
	map.graphics[ngraph].glyph = realloc(
	    map.graphics[ngraph].glyph,
	    sizeof( dr1Glyph *) * glyph);
	if (!strcmp( file, "db-indoor-dungeon-1")) {
	    map.graphics[ngraph].glyph[glyph].src = dr1_dung1;
	} else if (!strcmp( file, "db-indoor-dungeon-3")) {
	    map.graphics[ngraph].glyph[glyph].src = dr1_dung3;
	} else if (!strcmp( file, "db-indoor-castle-1")) {
	    map.graphics[ngraph].glyph[glyph].src = dr1_castle1;
	} else if (!strcmp( file, "db-objects-1")) {
	    map.graphics[ngraph].glyph[glyph].src = dr1_objects1;
	} else if (!strcmp( file, "db-npcs-2")) {
	    map.graphics[ngraph].glyph[glyph].src = dr1_npcs2;
	} else {
	    printf("Invalid source '%s' on line %d\n", file, line);
	    map.graphics[ngraph].glyph[glyph].src = NULL;
	}

	/* get row and col */
	sscanf(cpos, "%d %d", &row, &col);

        printf("%d %s %d %d\n", code, file, row, col);
    } while (!feof(fp) && *buf != 0);

    /* Get map data */
    sep=line;
    row=0;
    do {
	col=0; 
	fgets( buf, sizeof(buf), fp); line++;
	i = strlen(buf);
	if (buf[i-1] == '\n') buf[i-1] = 0;
	
	for( cpos=buf; *cpos; cpos+=2) {
	    if (!cpos[1]) {
		printf("Odd line length at line %d\n", line);
		break;
	    }
	    code = (cpos[0]<<8) + cpos[1];
	    
	    g = findgraphic( &map, code);
	    if (!g) {
		printf( "Unknown graphic '%c%c' on line %d\n", 
			cpos[0], cpos[1], line);
	    } else {
		printf( "Found graphic '%c%c' on line %d\n", 
			cpos[0], cpos[1], line);
	    }
	    map.grid.graphic[ row*map.grid.xsize + col] = g;
	    col++;
	}
	row++;
    } while (!feof(fp));

}

