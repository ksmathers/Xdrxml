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
struct dr1Monster;
struct dr1ItemType;
typedef struct {
    long value;			/* in copper */
    char* name;			
    int encumbrance;
    int inuse;
    int weapon;
    int identified;
    int uses;
    int count;

    struct dr1ItemType *type;
} dr1Item;

typedef void (* dr1Item_use_fnp) ( struct dr1Player *p, dr1Item *i, int function);
   /* action to perform when activating the item */

typedef int (* dr1Item_drop_fnp) ( struct dr1Player *p, dr1Item *i);
   /* action to perform on removing from inventory */

typedef void (* dr1Item_take_fnp) ( struct dr1Player *p, dr1Item *i);
   /* action to perform on adding to inventory */

typedef bool_t (* dr1Item_xdr_fnp) ( XDR *xdrs, char *node, dr1Item *i);
   /* save any persistent parts of the object */

typedef void (* dr1Item_copy_fnp) ( dr1Item* copy, dr1Item* orig);
   /* copy-constructor for non-unique items */

typedef void (* dr1Item_init_fnp) ( dr1Item* buf);
   /* default constructor */

typedef void (* dr1Item_attack_fnp) ( dr1Item *i, struct dr1Player *p, struct dr1Monster *m);
   /* action to perform on using a weapon for attack */

enum {
    DR1EQUIP_WEAPON,		/* Weapon hand */
    DR1EQUIP_GAUCHE,		/* Off hand */
    DR1EQUIP_ARMOR,		/* Body armor slot */
    DR1EQUIP_BACKPACK,		/* Return to backpack */

    DR1EQUIP_GAUNTLETS,		/* Gauntlets slot */
    DR1EQUIP_GIRDLE,		/* Girdle slot */
    DR1EQUIP_BRACERS,		/* Bracers slot */
    DR1EQUIP_LEFTRING,		/* Left hand ring slot */
    DR1EQUIP_RIGHTRING,		/* Right hand ring slot */
    DR1EQUIP_NECKLACE,		/* Necklace slot */
    DR1EQUIP_BELTHOOK		/* Belthook/pocket slot */
};

typedef void (* dr1Item_equip_fnp) ( dr1Item *i, struct dr1Player *p, int equip);
   /* action to perform on equipping a weapon or armor */

typedef struct dr1ItemType {
    long size;
    int code;
    int stackable;
    int unique;
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
 * dr1Item_tcode_size
 *
 *    The method finds the size of the real item 'i' including
 *    any extensions that may be in the derived subclass that
 *    the real object is.
 *
 *  PARAMETERS:
 *    tcode     The item to get the size of 
 *
 *  RETURNS:
 *    Size of the item in bytes
 *
 */
long dr1Item_tcode_size( int tcode);

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
 * xdr_dr1ItemPtr( xdrs, dr1Item**)
 */
bool_t xdr_dr1ItemPtr( XDR *xdrs, char *node, dr1Item**);

#endif
