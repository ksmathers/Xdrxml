#include "dice.h"
#include "tanner.h"
#include "armor.h"

/* global */
dr1Merchant dr1tanner;

/* initialization */
void
dr1Tanner_init() 
{
    dr1tanner.name="Frommer";
    dr1tanner.store="Frommer the Tanner's Shop";
    dr1tanner.mood=dr1Dice_roll( "d20");
    dr1tanner.bargainpoint = 2.1;
    dr1tanner.bestoffer = 0.55;

    dr1ItemSet_add( &dr1tanner.itemStore, &dr1Armor_leather.super);
}
