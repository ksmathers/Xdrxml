#include "weapon.h"
#include "money.h"
#include "dr1.h"

/*-------------------------------------------------------------------
 * dr1Weapon_type
 *
 *    Represents the item archetype for common weapons
 */
static bool_t xdr_dr1Weapon( XDR *xdrs, char *node, dr1Item *i);

dr1ItemType dr1Weapon_primary = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_BASICWEAPON,	/* code */
    FALSE,			/* stackable */
    FALSE,			/* unique */
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
    FALSE,			/* stackable */
    FALSE,			/* unique */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0,			/* init */
    (void *)0,                  /* copy */
    xdr_dr1Weapon,		/* xdr */
};

dr1ItemType dr1Weapon_missile = {
    sizeof( dr1Weapon),		/* size */
    (int)DR1W_MISSILE,		/* code */
    TRUE,			/* stackable */
    FALSE,			/* unique */
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
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* count       */ 1,
	/* type        */ &dr1Weapon_primary
    },
    /* damage   */ "d6",
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 4,
    /* dtype    */ DR1PIERCING,
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* missile      */ DR1W_NONE,
    /* min_str      */ 8
};

dr1Weapon dr1Weapon_longsword = {
    /* super */ {
    	/* value       */ GP(15),
	/* name        */ "Longsword",
	/* encumbrance */ 150,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* count       */ 1,
	/* type        */ &dr1Weapon_primary
    },
    /* damage   */ "d8",
    /* range    */ 0,
    /* rof      */ 0,
    /* speed    */ 6,
    /* dtype    */ DR1SLASHING,
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* missile      */ DR1W_NONE,
    /* min_str      */ 12
};

dr1Weapon dr1Weapon_shortbow = {
    /* super */ {
    	/* value       */ GP(35),
	/* name        */ "Shortbow",
	/* encumbrance */ 80,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* count       */ 1,
	/* type        */ &dr1Weapon_bow
    },
    /* damage   */ "0",
    /* range    */ 60,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING,
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* missile      */ DR1W_ARROW,
    /* min_str      */ 8
};

dr1Weapon dr1Weapon_longbow = {
    /* super */ {
    	/* value       */ GP(60),
	/* name        */ "Longbow",
	/* encumbrance */ 100,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* count       */ 1,
	/* type        */ &dr1Weapon_bow
    },
    /* damage   */ "1",
    /* range    */ 120,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING,
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ FALSE,
    /* missile      */ DR1W_ARROW,
    /* min_str      */ 16
};

dr1Weapon dr1Weapon_arrow = {
    /* super */ {
    	/* value       */ 10,
	/* name        */ "Arrow",
	/* encumbrance */ 1,
	/* inuse       */ FALSE,
	/* weapon      */ TRUE,
	/* identified  */ TRUE,
	/* uses        */ 1,
	/* count       */ 1,
	/* type        */ &dr1Weapon_missile
    },
    /* damage   */ "d6",
    /* range    */ 120,
    /* rof      */ 2,
    /* speed    */ 6,
    /* dtype    */ DR1PIERCING,
    /* plusToHit    */ 0,
    /* plusToDamage */ 0,
    /* stackable    */ TRUE,
    /* missile      */ DR1W_ARROW,
    /* min_str      */ 0
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
static bool_t xdr_dr1Weapon( XDR *xdrs, char *node, dr1Item *i) {
    bool_t res;
    int tmp;
    dr1Weapon *w = (dr1Weapon*)i;

    xdrxml_group( xdrs, node);
    
    if (!xdr_dr1Dice( xdrs, "damage", &w->damage)) return FALSE;
    if (!xdrxml_int( xdrs, "range", &w->range)) return FALSE;
    if (!xdrxml_int( xdrs, "rof", &w->rof)) return FALSE;
    if (!xdrxml_int( xdrs, "speed", &w->speed)) return FALSE;

    tmp = w->dtype;
    if (!xdrxml_int( xdrs, "dtype", &tmp)) return FALSE;
    w->dtype = tmp;

    if (!xdrxml_int( xdrs, "plusToHit", &w->plusToHit)) return FALSE;
    if (!xdrxml_int( xdrs, "plusToDamage", &w->plusToDamage)) return FALSE;
    if (!xdrxml_int( xdrs, "stackable", &w->stackable)) return FALSE;
    
    tmp = w->missile;
    if (!xdrxml_int( xdrs, "missile", &tmp)) return FALSE;
    w->missile = tmp;
    
    if (!xdrxml_int( xdrs, "min_str", &w->min_str)) return FALSE;

    xdrxml_endgroup( xdrs);
    return TRUE;
}

