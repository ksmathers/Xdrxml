#ifndef __DR1PHEAL__H
#define __DR1PHEAL__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "item.h"
#include "dice.h"
#include "player.h"
#ifndef __DR1GLOBALS__H
#   include "globals.h"
#endif

enum {
    DR1P_HEAL = DR1POTIONS
};
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
    dr1Dice effect;
} dr1pHeal;

/*-------------------------------------------------------------------
 * dr1pHeal_quaff
 *
 *    Use the potion
 *
 *  PARAMETERS:
 *    p   Player drinking the potion
 *    i   The potion being consumed
 *    fn  The mode being used
 *         0 = drink
 *         1 = taste
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    Increases the player hit points if damaged
 */
void dr1pHeal_quaff( dr1Player *p, dr1Item *i, int fn);


#endif /* __DR1PHEAL__H */
