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

enum DR1ProjectileType {
    DR1W_NONE,
    DR1W_ARROW, DR1W_BOLT, 
    DR1W_STONE, DR1W_BLOWDART, 
    DR1W_SPEAR, 
    DR1W_BOULDER
};
/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

extern dr1ItemType dr1Weapon_primary;
extern dr1ItemType dr1Weapon_bowType;

typedef struct {
    dr1Item super;
    dr1Dice damage;
    int range;		/* in feet, for ranged weapons */
    int rof;		/* rate of fire for ranged weapons */
    int speed;
    enum DR1DamageType dtype;
    int plusToHit;
    int plusToDamage;
    int stackable;
    enum DR1ProjectileType projectile;  /* type of projectile */
    int min_str;	/* minimum strength required to use */
} dr1Weapon;

extern dr1Weapon dr1Weapon_halfspear;
extern dr1Weapon dr1Weapon_longsword;
extern dr1Weapon dr1Weapon_longbow;


#endif /* __DR1WEAPON__H */
