#ifndef __DR1WEAPON__H
#define __DR1WEAPON__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

extern dr1ItemType dr1Weapon_type;

typedef struct {
    dr1Item super;
    dr1Dice damage;
    int ranged;
    int speed;
} dr1Weapon;

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

bool_t xdr_dr1Weapon( XDR *xdrs, dr1Item *i);

#endif /* __DR1WEAPON__H */
