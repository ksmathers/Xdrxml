#include <string.h>
#include "barter.h"
#include "merchant.h"
#include "dice.h"

#define min(a,b) (((a)<(b))?(a):(b))

/*-------------------------------------------------------------------
 * dr1Barter_create
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

dr1Barter* dr1Barter_create( 
    dr1Merchant* m, dr1Item* i) 
{
    dr1Barter *b = calloc( 1, sizeof(dr1Barter));
    
    b->merchant = m;
    b->item = i;
    b->start.cp = i->value;
    dr1Money_normalizex( &b->start, 0.0, m->ep);
    b->minsale = b->start;

    dr1Money_compound( &b->start, m->bargainpoint - 1.0, m->ep);
    dr1Money_compound( &b->minsale, m->bestoffer - 1.0, m->ep);
    b->lastoffer = b->minsale;

    return b;
}

/*-------------------------------------------------------------------
 * dr1Barter_startingOffer
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

char* dr1Barter_startingOffer() {
    return "That will come to";
}

/*-------------------------------------------------------------------
 * dr1Barter_offer
 *
 *    Make a new offer to the merchant.  
 *
 *  PARAMETERS:
 *    b     The barter agreement that is in progress
 *    v	    The clients next offer
 *
 *  RETURNS:
 *    String response of the merchant.  The money value is left for
 *    the caller to append to the return string.
 *
 *  SIDE EFFECTS:
 *    The barter object's 'start' is modified to reflect the current 
 *    price acceptable to the merchant.  The merchant's mood is also
 *    modified depending on the quality of the offer.
 */
char* dr1Barter_offer( dr1Barter *b, dr1Money o) {
    int userdelta = 0;
    int offer;
    int lastoffer;
    int start;
    int ep;		/* electrum mode */
    char *reply;

    ep = b->merchant->ep;
    offer = dr1Money_value( &o, ep);
    lastoffer = dr1Money_value( &b->lastoffer, ep);
    start = dr1Money_value( &b->start, ep);
    if ( offer == 0) {
	/*
	 * Client declines to purchase, let them bow out.
	 */
	return "Very well. Perhaps later.";
    }

    if ( offer <= lastoffer) {
    	/*
	 * Clients can't lower their offers or they'll prompt
	 * anger and a likewise increase from the merchant.
	 */
        dr1Money_compound( &b->start, 0.10, b->merchant->ep);
	dr1Merchant_makeAngry( b->merchant);
	return "Bah!  You insult me.  I'd not sell for less than";
    }

    userdelta = offer - lastoffer;
    b->lastoffer=o;
    
    if ( offer < dr1Money_value( &b->minsale, b->merchant->ep)) {
        /*
	 * Clients can't start insultingly low or they'll 
	 * anger the merchant and he'll raise his price.
	 */
	dr1Money_compound( &b->start, 0.10F, b->merchant->ep);
	dr1Merchant_makeAngry( b->merchant);
	reply = "Bah, you take me for a fool. I say";
    } else {
        /*
	 * The offer is reasonable.  Lets see how the
	 * merchant is feeling right now.
	 */
	int mood = b->merchant->mood; /* 0..20, large is angry */

	/* shifted bell curve */
	int rnd = min(dr1Dice_roll("3d10") + mood * 5, 99); 

	int span = start - dr1Money_value(&b->minsale, b->merchant->ep);
	int delta = span - (span * rnd / 100);
	dr1Money t;

	if (userdelta > 0) {
#if 0
	    printf( "delta %d .vs. %d", delta, userdelta);
#endif
	    if (delta > 2 * userdelta) dr1Merchant_makeAngry( b->merchant);
	    if (delta < userdelta) dr1Merchant_makeHappy( b->merchant);
	}

        bzero(&t, sizeof(t));
	t.cp = delta;
        dr1Money_normalizex( &t, 0.0, b->merchant->ep);
        dr1Money_deductx( &b->start, &t, b->merchant->ep);

	if (start - delta < offer) {
	    b->start = o;
	    reply = dr1Merchant_finalSale( b->merchant, offer, b->item);
	} else {
	    reply = dr1Merchant_sayCurrentOffer( b->merchant, rnd);
	}
    } // if
    return reply;
}
