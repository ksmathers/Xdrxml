#include "player.h"
#include "dice.h"
#include "potions/heal.h"

dr1ItemType dr1pHeal_type = {
    sizeof( dr1pHeal),		/* size */
    (int)'PHEA',		/* code */
    dr1pHeal_quaff, 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0 			/* xdr */
};

void dr1pHeal_quaff( dr1Player* p) {
    int h = dr1Dice_roll("d8");
    if (p->hp > p->full_hp) return;
    p->hp += h;
    if (p->hp > p->full_hp) p->hp=p->full_hp;
}

