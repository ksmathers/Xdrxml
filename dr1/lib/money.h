#ifndef __DR1MONEY__H
#define __DR1MONEY__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

#define GP(x) ((x)*200)

/*-------------------------------------------------------------------
 * dr1Money
 *
 *    The Money structure represents coinage of the realms.  
 *    Money comes in denominations of:
 *       10 copper (cp) = 1 silver (sp)
 *       20 silver (sp) = 1 gold (gp)
 *        2 electrum (ep) = 1 gold (gp)
 *        1 platinum (pp) = 5 gold (gp)
 *
 *    For determining encumbrance, all coins are considered the
 *    same weight, so exchanging coins for platinum is often a
 *    good idea when encountering a Money changer.  
 *
 *       10 coins = 1# (encumbrance)
 *
 *    There are two special cases.  Platinum is not normally
 *    traded, but will always be recognized and accepted by
 *    merchants and traders.  As a result you can never get
 *    gold traded up to platinum, but you can get it traded
 *    down to gold.
 *
 *    Electrum is only traded in certain parts of the world.
 *    Some places electrum will only be accepted, never made
 *    in change.  Elsewhere electrum can't be traded at all.
 *    And in some places electrum is traded in preference to
 *    gold.
 */

typedef struct {
    int cp;
    int sp;
    int ep;
    int gp;
    int pp;
} dr1Money;

/*-------------------------------------------------------------------
 * dr1Money_encumbrance
 *
 * Parameters:
 *    Money    	the set of Money to be weighed.
 *
 * Return:
 *    int 	weight of the Money in pounds ('#') of encumbrance.
 */
int dr1Money_encumbrance( dr1Money *money);

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
 *    long	the normalized value of the Money in copper
 */
long dr1Money_value( dr1Money *money, int ep);

/*-------------------------------------------------------------------
 * dr1Money_compound
 *
 * Parameters:
 *    money    	the set of Money to be revalued.
 *    ratio	ratio to increase by (negative to decrease)
 *    ep        Use electrum?
 *                0 - electrum is ignored
 *                1 - electrum is changed to gold/silver
 *                2 - electrum is accepted, compounded 
 *                    gold is converted to electrum
 */
void dr1Money_compound( dr1Money *money, float ratio, int ep);

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
void dr1Money_parse( dr1Money *purse, char *str);

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
void dr1Money_format( dr1Money *purse, char *buf);

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
void dr1Money_add( dr1Money *purse, const dr1Money *value);

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
 *
 * Side Effect:
 *    The purse is reduced by the value agreed upon as 'price'.
 *    Merchants will automatically make change if the agreed price
 *    includes coins that the player isn't currently carrying,
 *    but will not normalize the purse (see normalize).  Any 
 *    unavailable electrum is repaid in either gold or silver,
 *    since most merchants don't carry electrum.
 */
int dr1Money_deduct( dr1Money *purse, dr1Money *price);
int dr1Money_deductx( dr1Money *purse, dr1Money *price, int ep);

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
 *    Total charge for Money conversion.
 *
 * Side Effect:
 *    The purse is renormalized so that it only contains gp,
 *    sp, and cp.  Any existing pp remain untouched, but gp are
 *    not upwardly normalized to pp.  Electrum is traded in for 
 *    gold and silver.
 */
dr1Money dr1Money_normalize( dr1Money *purse, double charge);
dr1Money dr1Money_normalizex( dr1Money *purse, double charge, int ep);

/*-------------------------------------------------------------------
 * xdr_dr1Money( xdrs, dr1Money*)
 */
bool_t xdr_dr1Money( XDR *xdrs, char *node, dr1Money*);

#endif /* __DR1MONEY__H */
