/**
* Inventory item
*
*/
#include <assert.h>
#include "money.h"
#include "item.h"
#include "itemreg.h"
#include "xdrasc.h"

/*-------------------------------------------------------------------
 * dr1Item_destroy
 *
 *    Remove any storage attached to the Item .  The caller is still
 *    responsible for freeing the dr1Item itself if needed.
 *
 *  PARAMETERS:
 *    i     The item to destroy
 */

void
dr1Item_destroy( dr1Item *i) {
}

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
    char *s = 0;
    
    if (xdrs->x_handy & XDR_ANNOTATE) {
	s = (char *)xdrs->x_private;
	s += strlen( s);
    }

    if (xdrs->x_op == XDR_ENCODE) {
        code = i->type->code;
    }
    xdr_attr( xdrs, "code");
    if (!xdr_int( xdrs, &code)) return FALSE;
    printf("code %x\n", code);
    if (xdrs->x_op == XDR_DECODE) {
        i->type = dr1Registry_lookup( &dr1itemReg, code);
	assert(i->type);
    }

    xdr_attr( xdrs, "value");
    if (!xdr_long( xdrs, &i->value)) return FALSE;
   
    xdr_attr( xdrs, "name");
    if (!xdr_wrapstring( xdrs, &i->name)) return FALSE;
   
    xdr_attr( xdrs, "encumbrance");
    if (!xdr_int( xdrs, &i->encumbrance)) return FALSE;
   
    xdr_attr( xdrs, "unique");
    if (!xdr_int( xdrs, &i->unique)) return FALSE;

    xdr_attr( xdrs, "inuse");
    if (!xdr_int( xdrs, &i->inuse)) return FALSE;

    xdr_attr( xdrs, "weapon");
    if (!xdr_int( xdrs, &i->weapon)) return FALSE;

    xdr_attr( xdrs, "identified");
    if (!xdr_int( xdrs, &i->identified)) return FALSE;

    xdr_attr( xdrs, "uses");
    if (!xdr_int( xdrs, &i->uses)) return FALSE;

    if (i->type->xdr) {
	if (!i->type->xdr( xdrs, i)) return FALSE;
    }
    return TRUE;
}

