#ifndef __DR1TTYPE__H
#define __DR1TTYPE__H

#ifndef __DR1MONEY__H
#   include "money.h"
#endif
#ifndef __DR1DICE__H
#   include "dice.h"
#endif
/*-------------------------------------------------------------------
 * dr1
 *
 *    Random treasure generation 
 */

typedef struct {
    char ttype;

    int cp_chance;
    dr1Dice cp;
    int sp_chance;
    dr1Dice sp;
    int ep_chance;
    dr1Dice ep;
    int gp_chance;
    dr1Dice gp;
    int pp_chance;
    dr1Dice pp;

    int gems_chance;
    dr1Dice gems;

    int jewelry_chance;
    dr1Dice jewelry;
} dr1TType;

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

dr1TType* dr1TType_find( char ttype);

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

void dr1TType_collect( char* ttype, dr1Money *m, int *gems, int *jewelry);

#endif /* __DR1__H */
