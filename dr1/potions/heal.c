#include "player.h"
#include "dice.h"
#include "item.h"
#include "potions/heal.h"
#include "lib/xdrxml.h"

/*-------------------------------------------------------------------
 * xdr_dr1pHeal( xdrs, dr1Item*)
 */
static bool_t xdr_dr1pHeal( XDR *xdrs, dr1Item*);

/*-------------------------------------------------------------------
 * dr1pHeal_type
 *
 *    This global is the ItemType information for healing potions
 */
dr1ItemType dr1pHeal_type = {
    sizeof( dr1pHeal),		/* size */
    (int)DR1P_HEAL,		/* code */
    TRUE,			/* stackable */
    FALSE,			/* unique */
    dr1pHeal_quaff, 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0, 			/* init */
    (void *)0, 			/* copy */
    xdr_dr1pHeal,		/* xdr */
};

void dr1pHeal_quaff( dr1Player* p, dr1Item* i, int fn) {
    int h;
    dr1pHeal *pot = (dr1pHeal*)i;
    
    h = dr1Dice_roll(pot->effect);
    if (p->wounds <= 0) return;
    p->wounds -= h;
    if (p->wounds < 0) p->wounds = 0;
}


/*-------------------------------------------------------------------
 * xdr_dr1pHeal( xdrs, dr1pHeal*)
 */
static bool_t xdr_dr1pHeal( XDR *xdrs, dr1Item* p) {
    dr1pHeal *pot = (dr1pHeal*)p;

    xdr_attr( xdrs, "effect");
    return xdr_dr1Dice( xdrs, &pot->effect);
}

