#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "dice.h"
#include "lib/xdrxml.h"

double drand48();
void dr1Dice_seed() {
    srand48( time( NULL));
}
/*-------------------------------------------------------------------
 * dr1Dice_data
 *
 *    A function for getting the numerical values of a Dice object
 *
 * Parameters:
 *    ds	Dice string in standard notation ("3d6+1")
 *    ndice	Numbers of dice
 *    sides     Sides per die
 *    offset    Offset to the dice
 *    mult      Multiplier
 *
 * Returns:
 *    -1	Parse error
 *    0 	Success
 */

int dr1Dice_data( char *ds, int *nDice, int *sides, int *offset, int *multiple) 
{
    char pm = 0;
    int res;

    *nDice = 1;
    *sides = 0;
    *offset = 0;
    *multiple = 1;

    if (ds[0] == 'd') {
	*nDice=1;
	res = sscanf( ds, "d%d%c%d*%d", sides, &pm, offset, multiple);
    } else {
	res = sscanf( ds, "%dd%d%c%d*%d", nDice, sides, 
	              &pm, offset, multiple);
    }
    if (pm == '*') {
        *multiple = *offset; 
	*offset = 0;
    } 
    if (pm == '-') *offset = -*offset;
    return 0;
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
    int multiple = 1;
    int val = 0;

    int i;
    double d;

    dr1Dice_data( ds, &nDice, &sides, &offset, &multiple); 
    for (i=0; i<nDice; i++) {
        d = drand48();
	val += 1+floor(d * sides);
    }
    val += offset;
    val *= multiple;

/*    printf("Rolled %s for %d.\n", ds, val); /**/

    return val;
}

/*-------------------------------------------------------------------
 * xdr_dr1Dice( xdrs, dr1Dice*)
 */
bool_t xdr_dr1Dice( XDR *xdrs, dr1Dice* d) {
    xdr_attr(xdrs, "dice");
    return xdrxml_wrapstring( xdrs, d);
}

