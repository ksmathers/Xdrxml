#ifndef __DR1PLAYERV__H
#define __DR1PLAYERV__H

#ifndef __DR1PLAYER__H
#   include "player.h"
#endif
#ifndef __DR1CONTEXT__H
#   include "context.h"
#endif

enum { DIALOG_ERROR=-1, DIALOG_OK, DIALOG_DONE };

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

int dr1Playerv_cmd( dr1Context* ctx, int argc, char **argv);

void dr1Playerv_enable( dr1Context* ctx);
void dr1Playerv_disable( dr1Context* ctx);
#endif /* __DR1PLAYERV__H */
