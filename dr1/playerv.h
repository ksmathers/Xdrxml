#ifndef __DR1PLAYERV__H
#define __DR1PLAYERV__H

#ifndef __DR1PLAYER__H
#   include "player.h"
#endif
#ifndef __DR1CONTEXT__H
#   include "context.h"
#endif


/*-------------------------------------------------------------------
 * dr1
 *
 *    Opens the player generation dialog, and initializes p
 *
 *  PARAMETERS:
 *    p   The player to be initialized.  The caller should have 
 *        allocated this.
 *
 *  RETURNS:
 *    0   success
 *    -1  failure
 *
 *  SIDE EFFECTS:
 *    Initializes p from the player's selections
 */

int dr1Playerv_showDialog( dr1Context* ctx);

#endif /* __DR1PLAYERV__H */
