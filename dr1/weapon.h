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
    DR1W_BASICWEAPON = DR1WEAPONS,
    DR1W_BOW
};
/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

extern dr1ItemType dr1Weapon_type;
extern dr1ItemType dr1Weapon_bowType;

typedef struct {
    dr1Item super;
    dr1Dice damage;
    int ranged;
    int speed;
    enum DR1DamageType dtype;
} dr1Weapon;

extern dr1Weapon dr1Weapon_halfspear;
extern dr1Weapon dr1Weapon_longsword;
extern dr1Weapon dr1Weapon_longbow;


#endif /* __DR1WEAPON__H */
