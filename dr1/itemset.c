#include <assert.h>
#include "itemset.h"
#include "xdrasc.h"

/*-------------------------------------------------------------------
 * dr1ItemSetAdd
 *
 *    The method adds an item to the set.
 *
 *  PARAMETERS:
 *    i 	Pointer to malloc'd item
 *
 *  SIDE EFFECTS:
 *    The pointer is stored in the array for later retrieval.
 */
void dr1ItemSet_add( dr1ItemSet *set, dr1Item *i) {
    set->len++;
    if (set->len >= set->size) {
	if (set->size < 10) set->size = 10;
	set->size *= 2;
	set->items = realloc(set->items, sizeof(dr1Item*)*set->size);
	assert(set->items != 0);
    }
    set->items[set->len-1] = i;
}


/*-------------------------------------------------------------------
 * dr1ItemSet_remove
 *
 *    The method removes an item from the set.
 *
 *  PARAMETERS:
 *    i 	Pointer to item
 *
 *  SIDE EFFECTS:
 *    The pointer is removed from the array.  The caller is responsible
 *    for freeing the pointer.
 */
void dr1ItemSet_remove( dr1ItemSet* set, dr1Item *i) {
    int j;
    for (j=0; j<set->len; j++) {
	if (set->items[j] == i) {
	    set->items[j] = set->items[j+1];
	}
    }
    set->len--;
}

/*-------------------------------------------------------------------
 * dr1ItemSet_encumbrance
 *
 *    The method removes an item from the set.
 *
 *  PARAMETERS:
 *    set	The set to calculate
 *
 *  RETURNS:
 *    Returns encumbrance of the items in the item set in pounds.
 */
int dr1ItemSet_encumbrance( dr1ItemSet* set) {
    int i;
    int enc = 0;
    for ( i=0; i<set->len; i++) {
	enc += set->items[i]->encumbrance;
    }
    return enc;
}

/*-------------------------------------------------------------------
 * xdr_dr1ItemSet( xdrs, dr1ItemSet*)
 */
bool_t xdr_dr1ItemSet( XDR *xdrs, dr1ItemSet* set) {
    int i;
    long siz;

    xdr_attr( xdrs, "len");
    if (!xdr_int( xdrs, &set->len)) return FALSE;

    if (xdrs->x_op == XDR_DECODE) {
	set->size = set->len;
	set->items = malloc(sizeof(dr1Item*) * set->len);
    }
    for ( i=0; i<set->len; i++) {
        xdr_push_note( xdrs, "item-%d", i);
	if (xdrs->x_op == XDR_ENCODE) {
	    siz = dr1Item_size( set->items[i]);
	}
	xdr_attr( xdrs, "size");
	if (!xdr_long( xdrs, &siz)) return FALSE;
	if (xdrs->x_op == XDR_DECODE) {
	    set->items[i] = calloc( 1, siz);
	}
	if (!xdr_dr1Item( xdrs, set->items[i])) return FALSE;
	if (xdrs->x_op == XDR_FREE) {
	    free(set->items[i]);
	}
	xdr_pop_note( xdrs);
    }
    if (xdrs->x_op == XDR_FREE) {
	free( set->items);
    }
    return TRUE;
}

