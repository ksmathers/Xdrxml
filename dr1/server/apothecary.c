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
    dr1apothecary.name="Fnord";
    dr1apothecary.store="Fnord's Apothecary";
    dr1apothecary.mood=dr1Dice_roll( "d20");
    dr1apothecary.bargainpoint = 1.4;
    dr1apothecary.bestoffer = 0.85;

    i = calloc( 1, sizeof(dr1pHeal));
    i->super.value = GP(35);
    i->super.name = strdup("Healing-Potion");
    i->super.encumbrance = 10;
    i->super.inuse = FALSE;
    i->super.weapon = FALSE;
    i->super.identified = TRUE;
    i->super.uses = 1;
    i->super.count = 1;
    i->super.type = &dr1pHeal_type;
    i->effect = dr1Dice_create("d8");

    dr1ItemSet_add( &dr1apothecary.itemStore, (dr1Item*)i);

    i = calloc( 1, sizeof(dr1pHeal));
    i->super.value = GP(75);
    i->super.name = "Extra-Healing-Potion";
    i->super.encumbrance = 10;
    i->super.inuse = FALSE;
    i->super.weapon = FALSE;
    i->super.identified = TRUE;
    i->super.uses = 1;
    i->super.count = 1;
    i->super.type = &dr1pHeal_type;
    i->effect = dr1Dice_create("3d8");

    dr1ItemSet_add( &dr1apothecary.itemStore, (dr1Item*)i);
}

