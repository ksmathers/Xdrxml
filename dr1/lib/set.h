#ifndef __DR1SET__H
#define __DR1SET__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1Set
 *
 *    Represents a set of untyped objects; items can be added and 
 *    removed from the set.  The items array 'len' and 'size' must 
 *    be kept in synch with the items array itself so using the
 *    functions for adding and removing is recommended.
 *
 *    Generally routines assume that objects have been
 *    malloc'd, so don't pass in pointers to static data.  The
 *    xdr routines in particular wouldn't like it.
 */

typedef bool_t (*xdr_fn)( XDR *xdrs, void **p);
typedef int (*cmp_fn)( void* a, void *b);
typedef struct {
    xdr_fn xdr;
    cmp_fn cmp;
    int size;			/* allocated size of the items array */
    int len;			/* length of the array in use */
    void** items;
} dr1Set;

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
dr1Set* dr1Set_create( dr1Set* set, cmp_fn cmp, xdr_fn xdr);

/*-------------------------------------------------------------------
 * dr1Set_add
 *
 *    The method adds an item to the set.
 *
 *  PARAMETERS:
 *    i 	Pointer to malloc'd item
 *
 *  SIDE EFFECTS:
 *    The pointer is stored in the array for later retrieval.
 */
void dr1Set_add( dr1Set* set, void *i);

/*-------------------------------------------------------------------
 * dr1Set_find
 *
 *    Returns a pointer to an item in the itemset, searching by key
 *
 *  PARAMETERS:
 *    set       The itemset to look through
 *    key 	Key to look for
 *    cmp	Comparison function for callback
 *
 *  RETURNS:
 *    Pointer to the named item, or NULL.
 *
 */
void* dr1Set_find( dr1Set* set, void *key);

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
void dr1Set_remove( dr1Set* set, void *i);

/*-------------------------------------------------------------------
 * xdr_dr1Set( xdrs, dr1Set*)
 */
bool_t xdr_dr1Set( XDR *xdrs, dr1Set*);

#endif /* __DR1SET__H */
