#ifndef __DR1ARMOR__H
#define __DR1ARMOR__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#ifndef __DR1GLOBALS__H
#   include "globals.h"
#endif
#ifndef __DR1ITEM__H
#   include "item.h"
#endif
#ifndef __DR1DICE__H
#   include "dice.h"
#endif


/*-------------------------------------------------------------------
 * ItemType enumerations for armor types
 */
enum {
    DR1A_BASICARMOR = DR1ARMOR
};

/*-------------------------------------------------------------------
 * Armor subtypes
 */
enum {
    DR1A_LEATHER,
    DR1A_CHAINMAIL
};

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

extern dr1ItemType dr1Armor_type;

typedef struct {
    int code;
    char *name;
    dr1Dice damage;
    int base_ac;
} dr1ArmorType;

typedef struct {
    dr1Item super;
    dr1ArmorType *type;
    int damage;
} dr1Armor;

extern dr1Armor dr1Armor_leather;
extern dr1Armor dr1Armor_chainmail;

/*-------------------------------------------------------------------
 * dr1Armor_copy
 *
 *    The method copies an armor from a prototype.
 *
 *  PARAMETERS:
 *    dest    The destination storage
 *    source  The prototype armor object
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    Initializes the damage limit result->damage
 */

void dr1Armor_copy( dr1Item *dest, dr1Item *src);

#endif /* __DR1WEAPON__H */
