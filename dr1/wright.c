#include "dice.h"
#include "wright.h"
#include "weapon.h"

/* global */
dr1Merchant dr1wright;

/* initialization */
void
dr1Wright_init() 
{
    dr1wright.name="John Wright";
    dr1wright.store="John the wheel wright";
    dr1wright.mood=10 + dr1Dice_roll( "d10");
    dr1wright.bargainpoint = 1.25;
    dr1wright.bestoffer = 0.85;

    dr1ItemSet_add( &dr1wright.itemStore, &dr1Weapon_longbow.super);
}
