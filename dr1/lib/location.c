#include "location.h"
#include "dr1.h"

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

int dr1Location_distance( dr1Location *l1, dr1Location *l2) {
    int dist;
    int dx, dy;

    dx = l1->x - l2->x;
    dy = l1->y - l2->y;

    dist = sqrt(dx*dx + dy*dy) + 0.5;
    return dist;
}

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

dr1Location dr1Location_moveTo( dr1Location *_from, dr1Location *_to, int dist) {
    float dx, dy;
    dr1Location res;
    float theta;

    if (dr1Location_distance( _from, _to) <= dist) {
	res = *_to;
    } else {
	dx = _to->x - _from->x;
	dy = _to->y - _from->y;
	theta = atan2( dy, dx);
	
	res.x = dist * cos( theta) + _from->x + 0.5;
	res.y = dist * sin( theta) + _from->y + 0.5;
    }
    return res;
}

/*-------------------------------------------------------------------
 * xdr_dr1Location( xdrs, dr1Location*)
 */
bool_t xdr_dr1Location( XDR *xdrs, dr1Location* l) {
    xdr_attr( xdrs, "mapname");
    if (!xdrxml_wrapstring( xdrs, &l->mapname)) return FALSE;
    xdr_attr( xdrs, "x");
    if (!xdr_int( xdrs, &l->x)) return FALSE;
    xdr_attr( xdrs, "y");
    if (!xdr_int( xdrs, &l->y)) return FALSE;
    return TRUE;
}

/*-------------------------------------------------------------------
 * TEST MAIN
 */
#ifdef TEST
#include <stdio.h>
int main( int argc, char **argv) {
    dr1Location y = { 0, 10 };
    dr1Location o = { 0, 0 };
    dr1Location x = { 10, 0 };
    dr1Location r;
    int dist;

    dist = dr1Location_distance( &x, &y);
    printf("x to y dist = %d\n", dist);

    printf("x = ( %d, %d)\n", x.x, x.y);
    printf("y = ( %d, %d)\n", y.x, y.y);
    printf("o = ( %d, %d)\n", o.x, o.y);

    r = dr1Location_moveTo( &x, &o, 4);
    printf("x 4 to o = ( %d, %d)\n", r.x, r.y);
    r = dr1Location_moveTo( &y, &o, 4);
    printf("y 4 to o = ( %d, %d)\n", r.x, r.y);
    r = dr1Location_moveTo( &y, &x, dist/2);
    printf("y %d to x = ( %d, %d)\n", dist/2, r.x, r.y);
}
#endif
