#include <stdio.h>
#include <string.h>
#include "merchant.h"
#include "dice.h"

dr1Merchant dr1Merchant_create( 
	int _mood, float _bargainpoint, float _bestoffer) 
{
    dr1Merchant m;
    m.mood = _mood;
    m.bargainpoint = _bargainpoint;
    m.bestoffer = _bestoffer;
    return m;
}

dr1Item* dr1Merchant_inventory(dr1Merchant *m, char* name) 
{
    int i;
    for (i=0; i < m->itemStore.len; i++) {
	dr1Item *tmp = m->itemStore.items[i];
	if ( !strcmp(tmp->name, name)) return tmp;
    } // for
    return NULL;
}

void dr1Merchant_removeFromInventory( dr1Merchant *m, dr1Item *itm) {
    // itemStore.delete( itm);
}

void dr1Merchant_makeAngry( dr1Merchant *m) {
/*    printf("makeAngry\n"); /**/
    m->mood += dr1Dice_roll("d4");
    if (m->mood > 20) m->mood = 20;
}

void dr1Merchant_makeHappy( dr1Merchant *m) {
/*    printf("makeHappy\n"); /**/
    m->mood -= dr1Dice_roll("d4");
    if (m->mood < 0) m->mood = 0;
}

dr1Barter* dr1Merchant_buy( dr1Merchant *m,  char *item)
{
    dr1Barter *b;
    dr1Item* i = dr1Merchant_inventory( m, item);

    b = dr1Barter_create( m, i);
    return b;
}

/* caller must free result */
dr1Item* dr1Merchant_completeSale( dr1Merchant *m,  dr1Barter *b) 
{
    dr1Item *t, *i = b->item;
    long v = i->value;
    long sale = dr1Money_value( &b->start, m->ep);
    if (sale < v) {
	dr1Merchant_makeAngry(m);
    } else {
	dr1Merchant_makeHappy(m);
    }

    if (i->unique) {
	t = i;
	dr1Merchant_removeFromInventory( m, i);
    } else {
        dr1Item *t = malloc( sizeof(dr1Item));
	memcpy( t, i, sizeof(dr1Item));
    }
    return t;
}

char *dr1Merchant_finalSale(dr1Merchant *m, long sale, dr1Item *i)
{
    if ( sale < i->value) {
	return "Curse all pinchpennies. Well, take it then.";
    }

    if ( sale > i->value * 1.5) {
	return "Aye, that will do. May it serve thee well.";
    }
    return "You drive a hard bargain.";
}

char* dr1Merchant_sayCurrentOffer(dr1Merchant *m,  int scale) {
    if (scale < 60) {
	return "Life is short, and thou art long. What say thee to";
    }
    if (scale < 80) {
	return "Nay, not so little.  But I could let it go for";
    }
    if (scale < 90) {
	return "Tis less than I paid for it. Mayhap";
    }
    if (scale < 97) {
	return "Mercy m'Lord, I've five children to feed.  How about";
    }
    return "A pox on thy house. No less than";
}

