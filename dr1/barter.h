#ifndef __DR1BARTER__H
#define __DR1BARTER__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

#ifndef __DR1MONEY__H
#   include "money.h"
#endif

#include "item.h"

/*-------------------------------------------------------------------
 * dr1Barter
 *
 *    The structure ...
 */

typedef struct {
    dr1Money minsale;		/* minimum sale price */
    dr1Money lastoffer;		/* current offer */
    dr1Money start;		/* starting price */
    struct dr1Merchant* merchant;
    dr1Item *item;		/* item being purchased */
} dr1Barter;


/*-------------------------------------------------------------------
 * dr1Barter_create
 *
 *    Creates a new barter structure to buy the item 'i' from 
 *    the merchant 'm'.
 *
 *  PARAMETERS:
 *    m    The merchant
 *    i    The item being purchased
 *
 *  RETURNS:
 *    A malloc'd barter object.  Call free() on the barter object
 *    when you are done with it.
 */

dr1Barter* dr1Barter_create( struct dr1Merchant* m, dr1Item* i) ;

/*-------------------------------------------------------------------
 * dr1Barter_startingOffer
 *
 *    Returns a pointer to the static string that the merchant 
 *    will say as his initial offer.  
 *
 *  RETURNS:
 *    A static string (don't call free()).  The caller has to add
 *    the initial price (barter.start) to the end of the string.
 */

char* dr1Barter_startingOffer();

/*-------------------------------------------------------------------
 * dr1Barter_offer
 *
 *    Accepts money offer, and returns the merchant's reply.
 *
 *  PARAMETERS:
 *    b		The barter sale that is in progress
 *    v		The client's next offer
 *
 *  RETURNS:
 *    String text of the merchant's reply (the value of the
 *    reply is left for the caller to add to the end)
 *
 *  SIDE EFFECTS:
 *    The 'start' price of the merchant is updated to the merchants
 *    next acceptable offer.  If the client offers more than the
 *    merchant is willing to drop his offer then 'start' is set
 *    to the clients offer, and the string returned acknowleges 
 *    completion of the sale.  Also changes the mood of the
 *    merchant.
 */

char* dr1Barter_offer( dr1Barter *b, dr1Money v);

#endif /* __DR1BARTER__H */
