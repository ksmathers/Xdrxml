#ifndef __DR1TTYPE__H
#define __DR1TTYPE__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1
 *
 *    Random treasure generation 
 */

typedef struct {
    char ttype;

    dr1Dice pp_chance;
    dr1Dice pp;
    dr1Dice gp_chance;
    dr1Dice gp;
    dr1Dice ep_chance;
    dr1Dice ep;
    dr1Dice sp_chance;
    dr1Dice sp;
    dr1Dice cp_chance;
    dr1Dice cp;

    dr1Dice scroll_chance;
    dr1Dice scroll;
    dr1Dice miscmag_chance;
    dr1Dice miscmag;
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


#endif /* __DR1__H */
