#ifndef __DR1LOCATION__H
#define __DR1LOCATION__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include <math.h>
#include "map.h"

/*-------------------------------------------------------------------
 * dr1Location
 *
 *    The structure ...
 */

typedef struct {
    char *mapname;
    int x;
    int y;
} dr1Location;


/*-------------------------------------------------------------------
 * dr1Location_distance
 *
 *    The method calculates the distance between two locations.
 *
 *  PARAMETERS:
 *    l1, l2   The two locations
 *
 *  RETURNS:
 *    The distance between them rounded to the nearest int.
 *
 *  SIDE EFFECTS:
 */

int dr1Location_distance( dr1Location *l1, dr1Location *l2);

/*-------------------------------------------------------------------
 * dr1Location_moveTo
 *
 *    The method calculates the distance between two locations.
 *
 *  PARAMETERS:
 *    _from, _to   Where to move from and to
 *    dist	   Distance to traverse
 *
 *  RETURNS:
 *    A new Location 10 feet in the direction of _to.
 *
 *  SIDE EFFECTS:
 */

dr1Location dr1Location_moveTo( dr1Location *_from, dr1Location *_to, int dist);

/*-------------------------------------------------------------------
 * xdr_dr1Location( xdrs, dr1Location*)
 */
bool_t xdr_dr1Location( XDR *xdrs, char *node, dr1Location*);

#endif /* __DR1LOCATION__H */
