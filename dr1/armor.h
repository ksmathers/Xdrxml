#ifndef __DR1ARMOR__H
#define __DR1ARMOR__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#ifndef __DR1ITEM__H
#   include "item.h"
#endif
#ifndef __DR1DICE__H
#   include "dice.h"
#endif

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

extern dr1ArmorType dr1ArmorType_leather;

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

bool_t xdr_dr1Armor( XDR *xdrs, dr1Item *i);

#endif /* __DR1WEAPON__H */
