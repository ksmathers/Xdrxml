#include "weapon.h"
#include "money.h"
#include "xdrxml.h"

/*-------------------------------------------------------------------
 * dr1Weapon_type
 *
 *    Represents the item archetype for common weapons
 */
static bool_t xdr_dr1Weapon( XDR *xdrs, dr1Item *i);

dr1ItemType dr1Weapon_primary = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_BASICWEAPON,	/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0,			/* init */
    (void *)0,                  /* copy */
    xdr_dr1Weapon,		/* xdr */
};

dr1ItemType dr1Weapon_bow = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_BOW,		/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0,			/* init */
    (void *)0,                  /* copy */
    xdr_dr1Weapon,		/* xdr */
};

dr1ItemType dr1Weapon_arrow_t = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_ARROW,		/* code */
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
	/* type        */ &dr1Weapon_primary
    },
    /* damage   */ "d6",
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 4,
    /* dtype    */ DR1PIERCING
#if 0
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* projectile   */ DR1W_NONE,
    /* min_str      */ 8
#endif
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
	/* type        */ &dr1Weapon_primary
    },
    /* damage   */ "d8",
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 6,
    /* dtype    */ DR1SLASHING
#if 0
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* projectile   */ DR1W_NONE,
    /* min_str      */ 12
#endif
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
	/* type        */ &dr1Weapon_bow
    },
    /* damage   */ "d6",
    /* range    */ 120,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING
#if 0
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* projectile   */ DR1W_ARROW,
    /* min_str      */ 16
#endif
};

dr1Weapon dr1Weapon_arrow = {
    /* super */ {
    	/* value       */ 1,
	/* name        */ "Arrow",
	/* encumbrance */ 1,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 1,
	/* type        */ &dr1Weapon_arrow_t
    },
    /* damage   */ "d6",
    /* range    */ 120,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING
#if 0
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ TRUE,
    /* projectile   */ DR1W_ARROW,
    /* min_str      */ 0
#endif
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

