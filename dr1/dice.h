#ifndef __DR1DICE__H
#define __DR1DICE__H

#include <string.h>
#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1Dice 
 *
 *    This structure represents a set of dice.
 */
typedef char* dr1Dice;

/*-------------------------------------------------------------------
 * dr1Dice_create
 *
 *    Returns a dr1Dice created from the string 's'.  The caller
 *    is responsible for freeing the dice when done.
 */
#define dr1Dice_create(s) strdup(s);

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
 *    0..N	Random result of Dice throw
 */

int dr1Dice_data( char *ds, int *nDice, int *sides, int *offset, 
	int *multiple) ;

/*-------------------------------------------------------------------
 * dr1Dice_roll
 *
 *    A function for rolling standard polyhedra.  If the string doesn't
 *    parse correctly the results might be unusual.  The following 
 *    should work for example:
 *       "d8"
 *       "d100"
 *       "3d6"
 *       "3d4+2"
 *       "dr1-4"
 *       "3d4*10"
 *
 * Parameters:
 *    ds	Dice string in standard notation ("3d6+1")
 *
 * Returns:
 *    0..N	Random result of Dice throw
 */

int dr1Dice_roll( char *ds);

/*-------------------------------------------------------------------
 * dr1Dice_seed
 *
 *    Seeds the random number generator
 *
 */

void dr1Dice_seed();

/*-------------------------------------------------------------------
 * xdr_dr1Dice( xdrs, dr1Dice*)
 */
bool_t xdr_dr1Dice( XDR *xdrs, dr1Dice*);

#endif /* _DR1DICE__H */

