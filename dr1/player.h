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



typedef struct dr1Player {
    char *name;			/* Character name */
    long xp;			/* Experience points */

    int hp;			/* Hit points */
    int full_hp;		/* Full undamaged hits */

    dr1Attr base_attr;		/* Unmodified Attributes */
    dr1Attr curr_attr;		/* Attributes (incl. temporary effects) */

    dr1Money purse;		/* Money that Player is holding */
    dr1ItemSet pack;		/* Backpack equipment */

    int race;			/* Elf, Human, ... */
    int sex;			/* Male, Female */
} dr1Player;


/*-------------------------------------------------------------------
 * dr1Player_load
 *
 *    Load a player object from disk
 *
 *  PARAMETERS:
 *    p    Player buffer, or NULL
 *    fn   Filename to be loaded
 *
 *  RETURNS:
 *    Pointer to the dr1Player structure passed in, or NULL 
 *    if there was a problem loading the file.  If p is NULL
 *    a new player structure is malloc'd and returned.
 *
 *  SIDE EFFECT:
 *    Several substructures are malloc'd so the caller should
 *    dr1Player_destroy() the structure before disposing of
 *    it.
 */
dr1Player *dr1Player_load( dr1Player *p, char* fname);

/*-------------------------------------------------------------------
 * dr1Player_destroy
 *
 *    Destroy a malloc'd dr1Player structure.
 *
 *  PARAMETERS:
 *    p    Player structure to destroy
 */
void dr1Player_destroy( dr1Player *);

/*-------------------------------------------------------------------
 * dr1Player_encumbrance
 *
 *    Calculates the encubrance that the Player is carrying in pounds.
 */

int dr1Player_encumbrance();


/*-------------------------------------------------------------------
 * bool_t xdr_dr1Player( xdrs, dr1Player*)
 *   Returns TRUE on success
 */
bool_t xdr_dr1Player( XDR *xdrs, dr1Player*);

#endif /* __DR1PLAYER__H */
