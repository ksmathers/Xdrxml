#include "item.h"
#include "money.h"
#include "barter.h"
#include "itemset.h"

typedef struct dr1Merchant
{
    int mood;  		/* 0 - cheerful, 10 - annoyed, 20 - angry */
    float bargainpoint; /* starting offer (times intrinsic value) */
    float bestoffer;	/* about 1.0 (times intrinsic value) */
    int ep;		/* does merchant accept electrum? */

    dr1ItemSet itemStore;
} dr1Merchant;

dr1Item* dr1Merchant_inventory( dr1Merchant *m, char *name);

void dr1Merchant_removeFromInventory( dr1Merchant *m, dr1Item *itm);

int dr1Merchant_getMood(dr1Merchant *m );

void dr1Merchant_makeAngry(dr1Merchant *m );

void dr1Merchant_makeHappy(dr1Merchant *m );

dr1Barter *dr1Merchant_buy( dr1Merchant *m, char *item);

dr1Item *dr1Merchant_completeSale( dr1Merchant *m, dr1Barter *b);

char* dr1Merchant_finalSale( dr1Merchant *m, long offer, dr1Item *i);

char* dr1Merchant_sayCurrentOffer( dr1Merchant *m, int scale);
