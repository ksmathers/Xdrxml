#include <assert.h>
#include <string.h>
#include "set.h"
#include "xdrxml.h"

/*-------------------------------------------------------------------
 * dr1Set_create
 *
 *    Create or initialize a new set object
 *
 *  PARAMETERS:
 *    set	Static object to initialize, or NULL to allocate
 *    xdr	Serializer for the objects in the set
 *    cmp	Comparison for the objects in the set
 *
 *  SIDE EFFECTS:
 *    
 */
dr1Set* dr1Set_create( dr1Set* set, cmp_fn cmp, xdr_fn xdr) {
    if (!set) set=malloc( sizeof(*set));
    bzero( set, sizeof(*set));
    set->cmp = cmp;
    set->xdr = xdr;
    return set;
}

/*-------------------------------------------------------------------
 * dr1Set_add
 *
 *    The method adds a pointer to the set
 *
 *  PARAMETERS:
 *    i 	Pointer to malloced object
 *
 *  SIDE EFFECTS:
 *    The pointer is stored in the array for later retrieval.
 */
void dr1Set_add( dr1Set *set, void *i) {
    set->len++;
    if (set->len >= set->size) {
	if (set->size < 10) set->size = 10;
	set->size *= 2;
	set->items = realloc(set->items, sizeof(void*)*set->size);
	assert(set->items != 0);
    }
    set->items[set->len-1] = i;
}

/*-------------------------------------------------------------------
 * dr1Set_find
 *
 *    Returns a pointer to an item in the itemset, searching by name
 *
 *  PARAMETERS:
 *    set       The itemset to look through
 *    key       What to look for
 *    cmp       Callback function to compare the key
 *
 *  RETURNS:
 *    Pointer to the named item, or NULL.
 *
 */
void* dr1Set_find( dr1Set* set, void *key) {
    int j;
    for (j=0; j<set->len; j++) {
	if ( !set->cmp(set->items[j], key)) {
	    return set->items[j];
	}
    }
    return NULL;
}

/*-------------------------------------------------------------------
 * dr1Set_remove
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
void dr1Set_remove( dr1Set* set, void *i) {
    int j;
    for (j=0; j<set->len; j++) {
	if (set->items[j] == i) {
	    set->items[j] = set->items[j+1];
	}
    }
    set->len--;
}

/*-------------------------------------------------------------------
 * xdr_dr1Set( xdrs, dr1Set*)
 */
bool_t xdr_dr1Set( XDR *xdrs, char *node, dr1Set* set) {
    int i;

    xdrxml_group( xdrs, node);
    if (!xdrxml_int( xdrs, "len", &set->len)) return FALSE;

    if (xdrs->x_op == XDR_DECODE) {
	set->size = set->len;
	set->items = calloc(set->len, sizeof(void*));
    }
    for ( i=0; i<set->len; i++) {
        xdrxml_group( xdrs, "item");
	if (!set->xdr( xdrs, &set->items[i])) return FALSE;
        xdrxml_endgroup( xdrs);
    }
    if (xdrs->x_op == XDR_FREE) {
	free( set->items);
	set->items = NULL;
    }
    xdrxml_endgroup(xdrs);
    return TRUE;
}

