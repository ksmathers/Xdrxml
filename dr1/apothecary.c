#include "dice.h"
#include "apothecary.h"
#include "potions/heal.h"

/* global */
dr1Merchant dr1apothecary;

/* initialization */
void
dr1Apothecary_init() 
{
    dr1pHeal *i;
    dr1apothecary.mood=dr1Dice_roll( "dr1");
    dr1apothecary.bargainpoint = 1.4;
    dr1apothecary.bestoffer = 0.85;

    i = calloc( 1, sizeof(dr1pHeal));
    i->super.value = GP(55);
    i->super.name = "Healing Potion";
    i->super.encumbrance = 1;
    i->super.unique = FALSE;
    i->super.uses = 1;
    i->super.type = &dr1pHeal_type;

    dr1ItemSet_add( &dr1apothecary.itemStore, (dr1Item*)i);
}

