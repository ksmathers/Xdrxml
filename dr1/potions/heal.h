#ifndef __DR1PHEAL__H
#define __DR1PHEAL__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "item.h"
#include "player.h"

/*-------------------------------------------------------------------
 * dr1pHeal_type
 *
 *    This global holds the virtual methods for healing potions
 */
extern dr1ItemType dr1pHeal_type;

/*-------------------------------------------------------------------
 * dr1pHeal
 *
 *    This structure holds the healing potion storage
 */
typedef struct {
    dr1Item super;
} dr1pHeal;


/*-------------------------------------------------------------------
 * dr1pHeal_quaff
 *
 *    Use the potion
 *
 *  PARAMETERS:
 *    Player drinking the potion
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    Increases the player hit points if damaged
 */
void dr1pHeal_quaff( dr1Player *p);


#endif /* __DR1PHEAL__H */
