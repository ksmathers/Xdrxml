#ifndef __DR1ITEM__H
#define __DR1ITEM__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1Item
 *
 * Inventory item
 */

struct dr1Player;
struct dr1ItemType;
typedef struct {
    long value;			/* in copper */
    char* name;			
    int encumbrance;
    int unique;
    int uses;
    struct dr1ItemType *type;
} dr1Item;

typedef void (* dr1Item_use_fnp) ( struct dr1Player *p, dr1Item *i, int function);
typedef int (* dr1Item_drop_fnp) ( struct dr1Player *p, dr1Item *i);
typedef void (* dr1Item_take_fnp) ( struct dr1Player *p, dr1Item *i);
typedef bool_t (* dr1Item_xdr_fnp) ( XDR *xdrs, dr1Item *i);
typedef struct dr1ItemType {
    long size;
    int code;
    dr1Item_use_fnp use;
    dr1Item_drop_fnp drop;
    dr1Item_take_fnp take;
    dr1Item_xdr_fnp xdr;
} dr1ItemType;

/*-------------------------------------------------------------------
 * dr1Item_size
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
long dr1Item_size( dr1Item *i);

/*-------------------------------------------------------------------
 * xdr_dr1Item( xdrs, dr1Item*)
 */
bool_t xdr_dr1Item( XDR *xdrs, dr1Item*);

#endif
