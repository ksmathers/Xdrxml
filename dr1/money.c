#include <math.h>
#include <string.h>
#include <ctype.h>
#include "money.h"

/*-------------------------------------------------------------------
 * dr1Money_encumbrance
 *
 * Parameters:
 *    Money    	the set of Money to be weighed.
 *
 * Return:
 *    int 	weight of the Money in pounds ('#') of encumbrance.
 */
int dr1Money_encumbrance( dr1Money *Money) {
    return (Money->pp + Money->gp + Money->ep + Money->sp + Money->cp + 5)/10;
}

/*-------------------------------------------------------------------
 * dr1Money_value
 *
 * Parameters:
 *    Money    	the set of Money to be valued.
 *    ep        Use electrum?
 *                0 - electrum doesn't count here
 *                1 or 2 - electrum is accepted
 *
 * Return:
 *    long	the normalized value of the Money in cp
 */
long dr1Money_value( dr1Money *money, int ep) {
    return ( ( ( (money->pp * 5 + money->gp) * 2 + ( ep?money->ep:0 )
	       ) * 10 + money->sp
             ) * 10 + money->cp
	   );
}

/*-------------------------------------------------------------------
 * dr1Money_add
 *
 * Parameters:
 *    purse    the players Money
 *    value    the agreed upon return
 *
 * Side effect:
 *    purse contains the result of adding value to the purse
 */
void dr1Money_add( dr1Money *purse, const dr1Money *value) {
    purse->pp += value->pp;
    purse->gp += value->gp;
    purse->ep += value->ep;
    purse->sp += value->sp;
    purse->cp += value->cp;
}

/*-------------------------------------------------------------------
 * dr1Money_deduct
 *
 * Parameters:
 *    purse    	the players Money
 *    price     the agreed upon price
 *    ep	use electrum pieces?
 *                0 - the merchant will not accept electrum, but will
 *                    take the equivalent in gold/silver instead
 *                1 - the merchant will accept electrum, but can't
 *                    make change in electrum.  Any electrum change
 *                    will be given in gold/silver instead (default)
 *                2 - the merchant trades in electrum in preference
 *                    to gold.  Any gold change will be converted to
 *                    electrum.
 *
 * Return:
 *    0		success
 *    -1	insufficient funds in the purse
 *    -2        price in electrum, but merchant doesn't deal in electrum
 *    -3        invalid ep mode
 *
 * Side Effect:
 *    The purse is reduced by the value agreed upon as 'price'.
 *    Merchants will automatically make change if the agreed price
 *    includes coins that the player isn't currently carrying,
 *    but will not normalize the purse (see normalize).  Any 
 *    unavailable electrum is repaid in either gold or silver,
 *    since most merchants don't carry electrum.
 */
int dr1Money_deduct( dr1Money *purse, dr1Money *price) {
    return dr1Money_deductx( purse, price, 1);
}

/* ep mode 0 */
static int dr1Money_deduct0( dr1Money *purse, dr1Money *price) {
    int carry;

    if (price->ep != 0) return -2;

    purse->cp -= price->cp;
    carry = 0;
    if (purse->cp < 0) {
        /* not enough copper */
	carry = (-purse->cp + 9)/10; /* carry to silver */
	purse->cp += carry * 10;
    }

    purse->sp -= carry + price->sp;
    carry = 0;
    if (purse->sp < 0) {
	/* not enough silver */
	carry = (-purse->sp + 19)/20; /* carry to gold */
	purse->sp += carry * 20;
    }

    /* skip electrum entirely */

    purse->gp -= carry + price->gp;
    carry = 0;
    if (purse->gp < 0) {
	/* not enough gold */
	carry = (-purse->gp + 4)/5; /* carry to platinum */
	purse->gp += carry * 5;
    }

    purse->pp -= carry + price->pp;
    return 0;
}

/* ep mode 1 */
static int dr1Money_deduct1( dr1Money *purse, dr1Money *price) {
    int carry;

    purse->cp -= price->cp;
    carry = 0;
    if (purse->cp < 0) {
        /* not enough copper */
	carry = (-purse->cp + 9)/10; /* carry to silver */
	purse->cp += carry * 10;
    }

    purse->sp -= carry + price->sp;
    carry = 0;
    if (purse->sp < 0) {
	/* not enough silver */
	carry = (-purse->sp + 19)/20; /* carry to gold */
	purse->sp += carry * 20;
    }
    purse->gp -= carry;

    purse->ep -= price->ep;
    carry = 0;
    if (purse->ep < 0) {
	/* not enough electrum */
	carry = (-purse->ep + 1)/2; /* carry to gold */
	purse->ep += carry * 2;	
	purse->sp += purse->ep * 10;/* convert change to silver */
	purse->ep = 0;		    /* zero electrum */
    }

    purse->gp -= carry + price->gp;
    carry = 0;
    if (purse->gp < 0) {
	/* not enough gold */
	carry = (-purse->gp + 4)/5; /* carry to platinum */
	purse->gp += carry * 5;
    }

    purse->pp -= carry + price->pp;
    return 0;
}

/* ep mode 2 */
static int dr1Money_deduct2( dr1Money *purse, dr1Money *price) {
    int carry;

    purse->cp -= price->cp;
    carry = 0;
    if (purse->cp < 0) {
        /* not enough copper */
	carry = (-purse->cp + 9)/10; /* carry to silver */
	purse->cp += carry * 10;
    }

    purse->sp -= carry + price->sp;
    carry = 0;
    if (purse->sp < 0) {
	/* not enough silver */
	carry = (-purse->sp + 9)/10; /* carry to electrum */
	purse->sp += carry * 10;
    }

    purse->ep -= carry + price->ep;
    carry = 0;
    if (purse->ep < 0) {
	/* not enough electrum */
	carry = (-purse->ep + 1)/2; /* carry to gold */
	purse->ep += carry * 2;;	
    }

    purse->gp -= carry + price->gp;
    carry = 0;
    if (purse->gp < 0) {
	/* not enough gold */
	carry = (-purse->gp + 4)/5; /* carry to platinum */
	purse->gp += carry * 5;
	purse->ep += purse->gp * 2; /* convert gold change to electrum */
	purse->gp = 0;
    }

    purse->pp -= carry + price->pp;
    return 0;
}

int dr1Money_deductx( dr1Money *purse, dr1Money *price, int ep) {
    int res;

    if (dr1Money_value( purse, ep) < dr1Money_value( price, 1)) return -1;

    switch (ep) {
	case 0:
	    res = dr1Money_deduct0( purse, price);
	    break;
	case 1:
	    res = dr1Money_deduct1( purse, price);
	    break;
	case 2:
	    res = dr1Money_deduct2( purse, price);
	    break;
	default:
	    res = -3;
    }

    return res;
}

/*-------------------------------------------------------------------
 * dr1Money_parse
 *
 * Parameters:
 *    purse    the players Money
 *    str      string value
 *
 * Side effect:
 *    Initializes the purse from the parsed value of the string
 */
void dr1Money_parse( dr1Money *purse, char *str) {
    char *pos;
    char *s[5] = { "pp", "gp", "ep", "sp", "cp" };
    int v[5];
    int i;

    bzero( v, sizeof(v));
    for (i=0; i<5; i++) {
	pos = strstr( str, s[i]);
	if (!pos) continue;
	pos--;
	while ( isdigit(*pos)) pos--;
	pos++;
	v[i] = atoi(pos);
    }
    purse->pp = v[0];
    purse->gp = v[1];
    purse->ep = v[2];
    purse->sp = v[3];
    purse->cp = v[4];
}

/*-------------------------------------------------------------------
 * dr1Money_format
 *
 * Parameters:
 *    purse    the players Money
 *    buf      string value
 *
 * Side effect:
 *    Format the money object into a string in the buf buffer.
 */
void dr1Money_format( dr1Money *purse, char *buf) {
    char tbuf[80];
    buf[0] = 0;
    if (purse->pp) {
	sprintf(tbuf, "%dpp ", purse->pp);
	strcat( buf, tbuf);
    }
    if (purse->gp) {
	sprintf(tbuf, "%dgp ", purse->gp);
	strcat( buf, tbuf);
    }
    if (purse->ep) {
	sprintf(tbuf, "%dep ", purse->ep);
	strcat( buf, tbuf);
    }
    if (purse->sp) {
	sprintf(tbuf, "%dsp ", purse->sp);
	strcat( buf, tbuf);
    }
    if (purse->cp) {
	sprintf(tbuf, "%dcp ", purse->cp);
	strcat( buf, tbuf);
    }
}

/*-------------------------------------------------------------------
 * dr1Money_normalize
 *
 *    Changes all coins to the highest denomination possible, so 
 *    200cp would become 1gp.  First though a fee is assessed 
 *    based on the excess number of coins in the purse.  Copper
 *    in excess of 10 cp for example could be reduced by 5% 
 *    to pay the Money changer's fee.  (Fixed fees should be
 *    deducted seperately.)
 *
 *    Gold is not upconverted to platinum, since Money changers don't
 *    keep platinum (platinum is only for the king).  We don't down
 *    convert platinum to gold because it is legal to hold and use
 *    platinum, but once you spend it you won't ever get it back.
 *
 *    Like fivehundred dollar notes, platinum is only ever freely 
 *    traded between kingdoms.
 *
 * Parameters:
 *    purse    	the players Money
 *    charge    deduction to the Money changer, 0.1 == 10%
 *    ep	use electrum pieces?
 *                0 - the Money changer will not accept electrum
 *                    electrum counts will be untouched.
 *                1 - the Money changer will accept electrum, but 
 *		      doesn't make change in electrum.  Electrum
 *                    will be completely converted to gold/silver.
 *                    (default)
 *                2 - the Money changer trades in electrum.  Silver
 *                    will be up-converted to electrum, not to gold.
 *                    Gold and platinum will be left untouched.
 *
 * Return:
 *    Charges for Money-changing
 *
 * Side Effect:
 *    The purse is renormalized so that it only contains gp,
 *    sp, and cp.  Any existing pp remain untouched, but gp are
 *    not upwardly normalized to pp.  Electrum is traded in for 
 *    gold and silver.
 */
dr1Money dr1Money_normalize( dr1Money *purse, double charge) {
    return dr1Money_normalizex( purse, charge, 1);
}

dr1Money dr1Money_normalizex( dr1Money *purse, double charge, int ep) {
    int carry;
    dr1Money chg = { 0, 0, 0, 0, 0};

    carry = purse->cp / 10;		/* carry to silver */
    chg.sp = ceil(carry * charge);
    carry -= chg.sp;
    purse->cp = purse->cp % 10;
    purse->sp += carry;
    
    if (ep == 0 || ep == 1) {
	carry = purse->sp / 20;		/* carry to gold */
	chg.gp = ceil(carry * charge);
	carry -= chg.gp;
	purse->sp = purse->sp % 20;
	purse->gp += carry;
    } else {
	carry = purse->sp / 10;		/* carry to electrum */
	chg.ep = ceil(carry * charge);
	carry -= chg.ep;
	purse->sp = purse->sp % 10;
	purse->ep += carry;
    }
    
    if (ep == 1) {
	chg.ep = ceil( purse->ep * charge); /* convert ep to gold/silver */
	purse->ep -= chg.ep;
	carry = purse->ep / 2;
	purse->sp += (purse->ep % 2) * 10;
	purse->ep = 0;
	purse->gp += carry;
    }

    /* 
     * Gold is not upconverted to platinum
     */
    return chg;
}

/*-------------------------------------------------------------------
 * xdr_dr1Money( xdrs, dr1Money*)
 */
bool_t xdr_dr1Money( XDR *xdrs, dr1Money* p) {
    bool_t res;
    res = xdr_int( xdrs, &p->pp);
    res |= xdr_int( xdrs, &p->gp);
    res |= xdr_int( xdrs, &p->ep);
    res |= xdr_int( xdrs, &p->sp);
    res |= xdr_int( xdrs, &p->cp);
    return res;
}

/*-------------------------------------------------------------------
 * dr1Money_compound
 *
 * Parameters:
 *    Money    	the set of Money to be revalued.
 *    ep        Use electrum?
 *                0 - electrum is ignored
 *                1 - electrum is changed to gold/silver
 *                2 - electrum is accepted, compounded 
 *                    gold is converted to electrum
 */
void dr1Money_compound( dr1Money *money, float ratio, int ep) {
    dr1Money delta;
    bzero( &delta, sizeof(delta));
    delta.cp = dr1Money_value( money, ep) * fabs(ratio);
    dr1Money_normalizex( &delta, 0.0, ep);

    if (ratio < 0) {
	dr1Money_deductx( money, &delta, ep);
    } else {
        dr1Money_add( money, &delta);
    }
}

