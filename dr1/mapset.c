#include <string.h>
#include "mapset.h"
#include "set.h"

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

struct dr1MapSet {
    dr1Set maps;
} *dr1mapset;

static int cmp( void *a, void *b) {
    dr1Map *ma, *mb;
    ma = (dr1Map*)a; mb = (dr1Map*)b;
    return strcasecmp(ma->name, mb->name);
}

void dr1mapset_init( void) {
    dr1mapset = malloc( sizeof(*dr1mapset));
    dr1Set_create( &dr1mapset->maps, cmp, (void*)0);
}

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

dr1Map *dr1MapSet_selectmap( char *mapname) {
    dr1Map *m, mt;
    mt.name = mapname;
    if (!dr1mapset) dr1mapset_init();
    m = dr1Set_find( &dr1mapset->maps, &mt);
    if (!m) {
	m = dr1Map_readmap( mapname);
	if (m) dr1Set_add( &dr1mapset->maps, m);
    }
    m->select++;
    return m;
}

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

void dr1MapSet_dropmap( dr1Map *map) {
    map->select--;
    if (!map->select) {
	dr1Set_remove( &dr1mapset->maps, map);
	dr1Map_save( map);
	dr1Map_free( map);
    } /* if */
}



