#ifndef __DR1WEAPON__H
#define __DR1WEAPON__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#ifndef __DR1ITEM__H
#   include "item.h"
#endif
#ifndef __DR1DICE__H
#   include "dice.h"
#endif
#ifndef __DR1GLOBALS__H
#   include "globals.h"
#endif

enum {
    DR1W_BASICWEAPON = DR1WEAPONS
};
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

extern dr1Weapon dr1Weapon_halfspear;
extern dr1Weapon dr1Weapon_longsword;


#endif /* __DR1WEAPON__H */
