#ifndef __DR1ITEMSET__H
#define __DR1ITEMSET__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "item.h"

/*-------------------------------------------------------------------
 * dr1ItemSet
 *
 *    Represents a set of item objects; items can be added and 
 *    removed from the set.  The items array 'len' and 'size' must 
 *    be kept in synch with the items array itself so using the
 *    functions for adding and removing is recommended.
 *
 *    Generally routines assume that item objects have been
 *    malloc'd, so don't pass in pointers to static data.  The
 *    xdr routines in particular wouldn't like it.
 */

typedef struct {
    int size;			/* allocated size of the items array */
    int len;			/* length of the array in use */
    dr1Item** items;
} dr1ItemSet;

/*-------------------------------------------------------------------
 * dr1ItemSet_add
 *
 *    The method adds an item to the set.
 *
 *  PARAMETERS:
 *    i 	Pointer to malloc'd item
 *
 *  SIDE EFFECTS:
 *    The pointer is stored in the array for later retrieval.
 */
void dr1ItemSet_add( dr1ItemSet* set, dr1Item *i);

/*-------------------------------------------------------------------
 * dr1ItemSet_findName
 *
 *    Returns a pointer to an item in the itemset, searching by name
 *
 *  PARAMETERS:
 *    set       The itemset to look through
 *    name 	Name to look for
 *
 *  RETURNS:
 *    Pointer to the named item, or NULL.
 *
 */
dr1Item* dr1ItemSet_findName( dr1ItemSet* set, char *name);

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
void dr1ItemSet_remove( dr1ItemSet* set, dr1Item *i);

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
int dr1ItemSet_encumbrance( dr1ItemSet* set);

/*-------------------------------------------------------------------
 * xdr_dr1ItemPtr( xdrs, dr1Item**)
 */
bool_t xdr_dr1ItemPtr( XDR *xdrs, dr1Item**);

/*-------------------------------------------------------------------
 * xdr_dr1ItemSet( xdrs, dr1ItemSet*)
 */
bool_t xdr_dr1ItemSet( XDR *xdrs, dr1ItemSet*);

#endif /* __DR1ITEMSET__H */
