#include "player.h"
#include "dice.h"
#include "item.h"
#include "potions/heal.h"
#include "xdrasc.h"

dr1ItemType dr1pHeal_type = {
    sizeof( dr1pHeal),		/* size */
    (int)'PHEA',		/* code */
    dr1pHeal_quaff, 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    xdr_dr1pHeal,		/* xdr */
};

void dr1pHeal_quaff( dr1Player* p, dr1Item* i, int fn) {
    int h;
    dr1pHeal *pot = (dr1pHeal*)i;
    
    h = dr1Dice_roll(pot->effect);
    if (p->hp > p->full_hp) return;
    p->hp += h;
    if (p->hp > p->full_hp) p->hp=p->full_hp;
}


/*-------------------------------------------------------------------
 * xdr_dr1pHeal( xdrs, dr1pHeal*)
 */
bool_t xdr_dr1pHeal( XDR *xdrs, dr1Item* p) {
    dr1pHeal *pot = (dr1pHeal*)p;

    xdr_attr( xdrs, "effect");
    return xdr_dr1Dice( xdrs, &pot->effect);
}
