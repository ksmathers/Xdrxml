#include "weapon.h"
#include "money.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

dr1ItemType dr1Weapon_type = {
    sizeof( dr1Weapon),		/* size */
    (int)'WEAP',		/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    xdr_dr1Weapon,		/* xdr */
};

dr1Weapon half_spear = {
    /* super */ {
    	/* value       */ GP(4),
	/* name        */ "Half spear",
	/* encumbrance */ 20,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Weapon_type
    },
    /* damage   */ "d6-1",
    /* ranged   */ FALSE,
    /* speed    */ 4
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
    res = xdr_dr1Dice( xdrs, &w->damage);
    if (!res) return FALSE;

    res = xdr_int( xdrs, &w->ranged);
    if (!res) return FALSE;

    res = xdr_int( xdrs, &w->speed);
    if (!res) return FALSE;
    return TRUE;
}

