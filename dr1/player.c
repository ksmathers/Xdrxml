#include "player.h"

/*-------------------------------------------------------------------
 * dr1Player_encumbrance
 *
 *    Calculates the encubrance that the Player is carrying in pounds.
 */

int dr1Player_encumbrance( dr1Player *p) {
    int lbs;
    lbs = dr1Money_encumbrance( &p->purse);
    return lbs;
}

/*-------------------------------------------------------------------
 * xdr_dr1Player( xdrs, dr1Player*)
 */
bool_t xdr_dr1Player( XDR *xdrs, dr1Player* p) {
   bool_t res;
   res = xdr_string( xdrs, &p->name, MAXNAMELEN );
   res |= xdr_long( xdrs, &p->xp);
   res |= xdr_int( xdrs, &p->hp);
   res |= xdr_int( xdrs, &p->full_hp);
   res |= xdr_dr1Attr( xdrs, &p->base_attr);
   res |= xdr_dr1Attr( xdrs, &p->curr_attr);
   res |= xdr_dr1Money( xdrs, &p->purse);
   res |= xdr_dr1ItemSet( xdrs, &p->pack);
   return res;
}

