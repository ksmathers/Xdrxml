/**
* Inventory item
*
*/
#include <assert.h>
#include "money.h"
#include "item.h"
#include "itemreg.h"
#include "xdrxml.h"
#include "armor.h"

/*-------------------------------------------------------------------
 * dr1itemReg
 *
 *    This registry contains the item types known to the system.
 *
 *    See 'itemreg.c' for the real definition.
 *
 * static dr1RegistryEntry e[] = {
 *    { (int)DR1P_HEAL, &dr1pHeal_type },
 *    { (int)DR1W_BASICWEAPON, &dr1Weapon_type },
 *    { (int)DR1A_BASICARMOR, &dr1Armor_type },
 *    { -1, 0 }
 * };
 *
 */

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
 * dr1Item_tcode_size
 *
 *    The method returns the sizeof the item structure including
 *    type specific data
 *
 *  PARAMETERS:
 *    tcode  The item type code to get the size of 
 *
 *  RETURNS:
 *    Size of the item in bytes
 *
 */
long dr1Item_tcode_size( int tcode) {
    dr1Item i; 
    i.type = dr1Registry_lookup( &dr1itemReg, tcode);
    if (i.type) return dr1Item_size( &i);
    printf("Invalid typecode %x\n", tcode);
    return 0;
}

/*-------------------------------------------------------------------
 * dr1Item_size
 *
 *    The method returns the sizeof the item structure including
 *    type specific data
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
 * dr1Item_isArmor
 *
 *    The method checks the item to see if it is one of the armor 
 *    subtypes of item
 *
 *  PARAMETERS:
 *    i     The item to check
 *
 *  RETURNS:
 *    TRUE  It is an armor
 *    FALSE Otherwise
 *
 */
int dr1Item_isArmor( dr1Item *i) {
    int res;

    res = ( DR1ARMOR <= i->type->code && i->type->code < DR1ARMOR_END );
    return res;
}

/*-------------------------------------------------------------------
 * dr1Item_dup
 *
 *    The method dup duplicates an existing item
 *
 *  PARAMETERS:
 *    orig	The original item
 *
 *  RETURNS:
 *    Pointer to the newly malloc'd duplicate of the original
 *
 */
dr1Item* dr1Item_dup( dr1Item *orig) {
    int size = dr1Item_size( orig);
    dr1Item* copy = calloc( 1, size);
    if ( orig->type->copy) {
        /* use the virtual copy constructor if available */
	orig->type->copy( copy, orig);
    } else {
        /* otherwise just do a shallow copy */
	memcpy( copy, orig, size);
    }
    return copy;

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
/*    printf("code %x\n", code); /**/
    if (xdrs->x_op == XDR_DECODE) {
        i->type = dr1Registry_lookup( &dr1itemReg, code);
	assert(i->type);
    }

    xdr_attr( xdrs, "value");
    if (!xdr_long( xdrs, &i->value)) return FALSE;
   
    xdr_push_note( xdrs, "name");
    if (!xdr_wrapstring( xdrs, &i->name)) return FALSE;
    xdr_pop_note( xdrs);
   
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

