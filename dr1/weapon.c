#include "weapon.h"
#include "money.h"
#include "xdrasc.h"

/*-------------------------------------------------------------------
 * dr1Weapon_type
 *
 *    Represents the item archetype for common weapons
 */

dr1ItemType dr1Weapon_type = {
    sizeof( dr1Weapon),		/* size */
    (int)'WEAP',		/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    xdr_dr1Weapon,		/* xdr */
};

dr1Weapon dr1Weapon_halfspear = {
    /* super */ {
    	/* value       */ GP(4),
	/* name        */ "Halfspear",
	/* encumbrance */ 20,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Weapon_type
    },
    /* damage   */ "d6",
    /* ranged   */ FALSE,
    /* speed    */ 4
};

dr1Weapon dr1Weapon_longsword = {
    /* super */ {
    	/* value       */ GP(4),
	/* name        */ "Longsword",
	/* encumbrance */ 150,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Weapon_type
    },
    /* damage   */ "d8",
    /* ranged   */ FALSE,
    /* speed    */ 6
};

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
bool_t xdr_dr1Weapon( XDR *xdrs, dr1Item *i) {
    bool_t res;
    dr1Weapon *w = (dr1Weapon*)i;
    xdr_push_note( xdrs, "damage");
    res = xdr_dr1Dice( xdrs, &w->damage);
    xdr_pop_note( xdrs);
    if (!res) return FALSE;

    xdr_attr( xdrs, "ranged");
    res = xdr_int( xdrs, &w->ranged);
    if (!res) return FALSE;

    xdr_attr( xdrs, "speed");
    res = xdr_int( xdrs, &w->speed);
    if (!res) return FALSE;
    return TRUE;
}

