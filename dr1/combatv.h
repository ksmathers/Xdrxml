#ifndef __DR1COMBATV__H
#define __DR1COMBATV__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#ifndef __DR1PLAYER__H
#   include "player.h"
#endif
#ifndef __DR1MONSTER__H
#   include "monster.h"
#endif

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

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

int attack( dr1Player *p, int nmon, dr1Monster *m, int c, char **v);

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

void dr1Combatv_showPage( dr1Player *p, int nmon, dr1Monster *m);

#endif /* __DR1COMBATV__H */
