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
#ifndef __DR1WEAPON__H
#   include "weapon.h"
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
    int level;			/* Training level */

    int hp;			/* Hit points */
    int full_hp;		/* Full undamaged hits */

    dr1Attr base_attr;		/* Unmodified Attributes */
    dr1Attr curr_attr;		/* Attributes (incl. temporary effects) */

    dr1Money purse;		/* Money that Player is holding */
    dr1ItemSet pack;		/* Backpack equipment */
    dr1Weapon* weapon;		/* Equipped weapon */
    dr1Item* gauche;		/* Offhand item */

    int race;			/* Elf, Human, ... */
    int sex;			/* Male, Female */
    int class;			/* MU, Thief, Fighter, Cleric, ... */
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
 * dr1Player_thac0
 *
 *    The method dr1Player_thac0 returns the to hit for AC 0 
 *    for the monster type.
 *
 *  PARAMETERS:
 *    p     The player in question
 *
 *  RETURNS:
 *    To hit
 *
 */

int dr1Player_thac0( dr1Player *p);

/*-------------------------------------------------------------------
 * dr1Player_ac
 *
 *    The method dr1Player_ac returns the effective ac of the 
 *    player, given certain attack conditions
 *
 *  PARAMETERS:
 *    p         The player in question
 *    surprise  Is the player surprised?
 *    ranged    Is it a ranged attack?
 *    dtype     Damage type
 *
 *  RETURNS:
 *    Armor Class
 *
 */

int dr1Player_ac( dr1Player *p, int surprise, int ranged, int dtype);

/*-------------------------------------------------------------------
 * bool_t xdr_dr1Player( xdrs, dr1Player*)
 *   Returns TRUE on success
 */
bool_t xdr_dr1Player( XDR *xdrs, dr1Player*);

#endif /* __DR1PLAYER__H */
