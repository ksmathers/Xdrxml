#include "dice.h"
#include "smithy.h"

/* global */
dr1Merchant dr1smithy;

/* initialization */
void
dr1Smithy_init() 
{
    dr1Item *i;
    dr1smithy.mood=dr1Dice_roll( "d20");
    dr1smithy.bargainpoint = 1.5;
    dr1smithy.bestoffer = 0.75;

    i = calloc( 2, sizeof(dr1Item));
    i[0].name = "Longsword";
    i[0].value = GP(300);
    i[1].name = "Broadsword";
    i[1].value = GP(350);
    dr1ItemSet_add( &dr1smithy.itemStore, &i[0]);
    dr1ItemSet_add( &dr1smithy.itemStore, &i[1]);
}
