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
 *    The method attack runs a single attack round for the character,
 *    attacking a monster.
 *
 *  PARAMETERS:
 *    p		Player
 *    nmon	Number of attackers
 *    m		Array of attackers
 *    surprise	Sets surprise to TRUE on a critical miss
 *    c		number of command line arguments
 *    v         command line argument list
 *              v[0] is the attack command ('attack')
 *              v[1] is the name of the monster to target
 *
 *  RETURNS:
 *    0		Success
 *    -1..-N 	Error
 *
 *  SIDE EFFECTS:
 *    Monster hit points are reduced, and surprise is set.
 */

int attack( dr1Player *p, int nmon, dr1Monster *m, int *surprise, int c, char **v);

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
