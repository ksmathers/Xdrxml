#ifndef __DR1PLAYER__H
#define __DR1PLAYER__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

#ifndef __DR1ATTR__H
#   include "attr.h"
#endif
#ifndef __DR1MONEY__H
#   include "money.h"
#endif
#ifndef __DR1ITEMSET__H
#   include "itemset.h"
#endif

#define MAXNAMELEN 1024

/*-------------------------------------------------------------------
 * dr1Player
 *
 *    The Player structure is the top level structure for all Attributes
 *    of a Player character.
 */

typedef struct {
    char *name;			/* Character name */
    long xp;			/* Experience points */

    int hp;			/* Hit points */
    int full_hp;		/* Full undamaged hits */

    dr1Attr base_attr;		/* Unmodified Attributes */
    dr1Attr curr_attr;		/* Attributes (incl. temporary effects) */

    dr1Money purse;		/* Money that Player is holding */
    dr1ItemSet pack;		/* Backpack equipment */
} dr1Player;

/*-------------------------------------------------------------------
 * dr1Player_encumbrance
 *
 *    Calculates the encubrance that the Player is carrying in pounds.
 */

int dr1Player_encumbrance();


/*-------------------------------------------------------------------
 * xdr_dr1Player( xdrs, dr1Player*)
 */
bool_t xdr_dr1Player( XDR *xdrs, dr1Player*);

#endif /* __DR1PLAYER__H */
