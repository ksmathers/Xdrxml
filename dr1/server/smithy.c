#include "dice.h"
#include "smithy.h"
#include "weapon.h"
#include "armor.h"

/* global */
dr1Merchant dr1smithy;

/* initialization */
void
dr1Smithy_init() 
{
    dr1smithy.name="Gandor";
    dr1smithy.store="Dragon's Reach Smithy";
    dr1smithy.mood=dr1Dice_roll( "d20");
    dr1smithy.bargainpoint = 1.5;
    dr1smithy.bestoffer = 0.75;

    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Weapon_halfspear.super);
    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Weapon_longsword.super);
    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Armor_scalemail.super);
    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Armor_chainmail.super);
    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Armor_splintmail.super);
    dr1ItemSet_add( &dr1smithy.itemStore, &dr1Armor_platemail.super);
}
