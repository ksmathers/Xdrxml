#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/param.h>
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

char *dr1GlyphTable_file( dr1GlyphTable *tbl) {
    return tbl->fname;
}

void dr1GlyphSet_init( void)
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
    }
}

