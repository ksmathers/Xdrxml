#ifndef __DR1DICE__H
#define __DR1DICE__H

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

#endif /* _DR1DICE__H */
