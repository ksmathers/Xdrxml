#include "weapon.h"
#include "money.h"
#include "xdrasc.h"

/*-------------------------------------------------------------------
 * dr1Weapon_type
 *
 *    Represents the item archetype for common weapons
 */
static bool_t xdr_dr1Weapon( XDR *xdrs, dr1Item *i);

dr1ItemType dr1Weapon_type = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_BASICWEAPON,	/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0,			/* init */
    (void *)0,                  /* copy */
    xdr_dr1Weapon,		/* xdr */
};

dr1ItemType dr1Weapon_bowType = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_BOW,		/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0,			/* init */
    (void *)0,                  /* copy */
    xdr_dr1Weapon,		/* xdr */
};

dr1Weapon dr1Weapon_halfspear = {
    /* super */ {
    	/* value       */ GP(1),
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
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 4,
    /* dtype    */ DR1PIERCING
};

dr1Weapon dr1Weapon_longsword = {
    /* super */ {
    	/* value       */ GP(15),
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
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 6,
    /* dtype    */ DR1SLASHING
};

dr1Weapon dr1Weapon_longbow = {
    /* super */ {
    	/* value       */ GP(60),
	/* name        */ "Longbow",
	/* encumbrance */ 100,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Weapon_type
    },
    /* damage   */ "d6",
    /* range    */ 120,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING
};

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method 
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
static bool_t xdr_dr1Weapon( XDR *xdrs, dr1Item *i) {
    bool_t res;
    int dt;
    dr1Weapon *w = (dr1Weapon*)i;
    xdr_push_note( xdrs, "damage");
    res = xdr_dr1Dice( xdrs, &w->damage);
    xdr_pop_note( xdrs);
    if (!res) return FALSE;

    xdr_attr( xdrs, "range");
    res = xdr_int( xdrs, &w->range);
    if (!res) return FALSE;

    xdr_attr( xdrs, "rof");
    res = xdr_int( xdrs, &w->rof);
    if (!res) return FALSE;

    xdr_attr( xdrs, "speed");
    res = xdr_int( xdrs, &w->speed);
    if (!res) return FALSE;

    xdr_attr( xdrs, "dtype");
    dt = w->dtype;
    res = xdr_int( xdrs, &dt);
    w->dtype = dt;
    if (!res) return FALSE;
    return TRUE;
}

