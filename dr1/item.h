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
    int inuse;
    int weapon;
    int identified;
    int uses;

    struct dr1ItemType *type;
} dr1Item;

typedef void (* dr1Item_use_fnp) ( struct dr1Player *p, dr1Item *i, int function);
   /* action to perform when activating the item */

typedef int (* dr1Item_drop_fnp) ( struct dr1Player *p, dr1Item *i);
   /* action to perform on removing from inventory */

typedef void (* dr1Item_take_fnp) ( struct dr1Player *p, dr1Item *i);
   /* action to perform on adding to inventory */

typedef bool_t (* dr1Item_xdr_fnp) ( XDR *xdrs, dr1Item *i);
   /* save any persistent parts of the object */

typedef void (* dr1Item_copy_fnp) ( dr1Item* copy, dr1Item* orig);
   /* copy-constructor for non-unique items */

typedef void (* dr1Item_init_fnp) ( dr1Item* buf);
   /* default constructor */

typedef struct dr1ItemType {
    long size;
    int code;
    dr1Item_use_fnp use;
    dr1Item_drop_fnp drop;
    dr1Item_take_fnp take;
    dr1Item_init_fnp init;
    dr1Item_copy_fnp copy;
    dr1Item_xdr_fnp xdr;
} dr1ItemType;

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
dr1Item* dr1Item_dup( dr1Item *i);

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
int dr1Item_isArmor( dr1Item *i);

/*-------------------------------------------------------------------
 * dr1Item_size
 *
 *    The method finds the size of the real item 'i' including
 *    any extensions that may be in the derived subclass that
 *    the real object is.
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
