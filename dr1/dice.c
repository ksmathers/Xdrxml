#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "dice.h"

double drand48();
void dr1Dice_seed() {
    srand48( time( NULL));
}
/*-------------------------------------------------------------------
 * dr1Dice_roll
 *
 *    A function for rolling standard polyhedra
 *
 * Parameters:
 *    ds	Dice string in standard notation ("3d6+1")
 *
 * Returns:
 *    -1	Parse error
 *    0..N	Random result of Dice throw
 */

int dr1Dice_roll( char *ds) {
    int nDice = 1;
    int sides = 0;
    int offset = 0;
    char pm = 0;
    int res;
    int val = 0;

    int i;
    double d;

    if (ds[0] == 'd') {
	nDice=1;
	res = sscanf( ds, "d%d%c%d", &sides, &pm, &offset);
    } else {
	res = sscanf( ds, "%dd%d%c%d", &nDice, &sides, &pm, &offset);
    }
    for (i=0; i<nDice; i++) {
        d = drand48();
	val += 1+floor(d * sides);
    }
    if (pm == '*') {
	val *= offset;
    } else {
	if (pm == '-') offset = -offset;
	val += offset;
    }

    return val;
}

/*-------------------------------------------------------------------
 * xdr_dr1Dice( xdrs, dr1Dice*)
 */
bool_t xdr_dr1Dice( XDR *xdrs, dr1Dice* d) {
    return xdr_wrapstring( xdrs, d);
}

