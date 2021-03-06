/**
* Inventory item
*
*/
#include <assert.h>
#include "dr1.h"
#include "money.h"
#include "item.h"
#include "itemreg.h"
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
 * xdr_dr1ItemPtr( xdrs, dr1Item**)
 */
static bool_t xdr_dr1Item( XDR *xdrs, char *node, dr1Item* i) {
    xdrxml_group( xdrs, node);
    if (!xdrxml_long( xdrs, "value", &i->value)) return FALSE;
    if (!xdrxml_wrapstring( xdrs, "name", &i->name)) return FALSE;
    if (!xdrxml_int( xdrs, "encumbrance", &i->encumbrance)) return FALSE;
    if (!xdrxml_int( xdrs, "inuse", &i->inuse)) return FALSE;
    if (!xdrxml_int( xdrs, "weapon", &i->weapon)) return FALSE;
    if (!xdrxml_int( xdrs, "identified", &i->identified)) return FALSE;
    if (!xdrxml_int( xdrs, "uses", &i->uses)) return FALSE;
    if (!xdrxml_int( xdrs, "count", &i->count)) return FALSE;

    if (i->type->xdr) {
	if (!i->type->xdr( xdrs, "variant", i)) return FALSE;
    }
    xdrxml_endgroup( xdrs);
    return TRUE;
}

/*-------------------------------------------------------------------
 * xdr_dr1ItemPtr( xdrs, dr1Item**)
 */
bool_t xdr_dr1ItemPtr( XDR *xdrs, char *node, dr1Item **itemp) {
    int tcode = 0;
    dr1ItemType *ityp;
    
    xdrxml_group( xdrs, node);
    /*
     * Get the virtual type pointer 
     */
    if (xdrs->x_op == XDR_ENCODE || xdrs->x_op == XDR_FREE) {
	if (*itemp) {
	    ityp = (*itemp)->type;
	    tcode = ityp->code;
	} else {
	    ityp = NULL;
	    tcode = DR1ILLEGAL;
	}
    } 
    
    if (!xdrxml_int32_t( xdrs, "code", &tcode)) { return FALSE; }

    if (xdrs->x_op == XDR_DECODE) {
	printf("got tcode 0x%x\n", tcode); /**/
	if (tcode == DR1ILLEGAL) {
	    ityp = NULL;
	} else {
	    ityp = dr1Registry_lookup( &dr1itemReg, tcode);
	    assert(ityp);
	    if (!ityp) return FALSE;
	}
    } /* decode */

    /*
     * Read the rest of the object
     */
    if (ityp) {
	/* Non-null object */
	if (xdrs->x_op == XDR_DECODE) {
	    *itemp = calloc( 1, ityp->size);
	    (*itemp)->type = ityp;
	}
	if (!xdr_dr1Item( xdrs, "item", *itemp)) return FALSE;
	if (xdrs->x_op == XDR_FREE) {
	    free(*itemp);
	    *itemp = NULL;
	}
    } else {
        /* Null object */
	*itemp = NULL;
    }
    xdrxml_endgroup( xdrs);

    return TRUE;
}

