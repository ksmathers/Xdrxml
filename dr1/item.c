/**
* Inventory item
*
*/
#include "money.h"
#include "item.h"
#include "itemreg.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *    i     The item to get the size of 
 *
 *  RETURNS:
 *    Size of the item in bytes
 *
 */
long dr1Item_size( dr1Item *i) {
    return i->type->size;
}

/*-------------------------------------------------------------------
 * xdr_dr1Item( xdrs, dr1Item*)
 */
bool_t xdr_dr1Item( XDR *xdrs, dr1Item* i) {
    int code;
    bool_t res;

    if (xdrs->x_op == XDR_ENCODE) {
        code = i->type->code;
    }
    res |= xdr_int( xdrs, &code);
    if (xdrs->x_op == XDR_DECODE) {
        i->type = dr1Registry_lookup( &dr1itemReg, code);
    }

    res |= xdr_long( xdrs, &i->value);
    res |= xdr_wrapstring( xdrs, &i->name);
    res |= xdr_int( xdrs, &i->encumbrance);
    res |= xdr_int( xdrs, &i->unique);
    res |= xdr_int( xdrs, &i->uses);
    if (i->type->xdr) {
	res |= i->type->xdr( xdrs, i);
    }
    return res;
}

