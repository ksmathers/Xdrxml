#include "registry.h"
#include "armor.h"
#include "xdrasc.h"
#include "item.h"
#include "money.h"

/*-------------------------------------------------------------------
 * dr1Armor_type
 *
 *    The global dr1Armor_type holds the ItemType data for 
 *    generic armor (generic armor all works the same)
 */

static bool_t xdr_dr1Armor( XDR *xdrs, dr1Item *i);
dr1ItemType dr1Armor_type = {
    sizeof( dr1Armor),		/* size */
    (int)DR1A_BASICARMOR,	/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    (void *)0, 			/* init */
    dr1Armor_copy, 		/* copy */

    xdr_dr1Armor,		/* xdr */
};

/*-------------------------------------------------------------------
 * dr1armortype
 *
 *    The global is a registry of the sub-types of armor within
 *    the armor class.  
 */

static dr1ArmorType leather = {
    /* code    */ DR1A_LEATHER,
    /* name    */ "Leather",
    /* damage  */ "3d10+20",
    /* base_ac */ 9
};

static dr1ArmorType chainmail = {
    /* code    */ DR1A_CHAINMAIL,
    /* name    */ "Chainmail",
    /* damage  */ "5d20+20",
    /* base_ac */ 5
};

static dr1RegistryEntry entries[] = {
    { DR1A_LEATHER, &leather },
    { DR1A_CHAINMAIL, &chainmail },
    { -1, NULL }
};

dr1Registry dr1armortype = {
    entries
};

/*-------------------------------------------------------------------
 * dr1Armor_leather
 * dr1Armor_chainmail
 *
 *    The global holds a prototype armor.  Use dr1Item_dup to get 
 *    a working copy, or dr1ItemStore_add to add the prototype 
 *    to a merchant inventory.
 */

dr1Armor dr1Armor_leather = {
    /* super */ {
    	/* value       */ GP(30),
	/* name        */ "Leather",
	/* encumbrance */ 100,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ FALSE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Armor_type
    },
    /* type     */ &leather,
    /* damage   */ 0
};

dr1Armor dr1Armor_chainmail = {
    /* super */ {
    	/* value       */ GP(170),
	/* name        */ "Chainmail",
	/* encumbrance */ 100,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ FALSE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Armor_type
    },
    /* type     */ &chainmail,
    /* damage   */ 0
};

/*-------------------------------------------------------------------
 * dr1Armor_copy
 *
 *    The method copies an armor from a prototype.
 *
 *  PARAMETERS:
 *    dest    The destination storage
 *    source  The prototype armor object
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    Initializes the damage limit result->damage
 */

void dr1Armor_copy( dr1Item* dest, dr1Item *source) {
    dr1Armor *adest = (dr1Armor*)dest;
    dr1Armor *asource = (dr1Armor*)source; 
    *adest = *asource;
    adest->damage = dr1Dice_roll( adest->type->damage);
}

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

static bool_t xdr_dr1Armor( XDR *xdrs, dr1Item *i) {
    dr1Armor *a = (dr1Armor *)i;
    int code;

    if ( xdrs->x_op == XDR_ENCODE) {
	code = a->type->code;
    }

    xdr_attr( xdrs, "type");
    if ( !xdr_int( xdrs, &code)) return FALSE;

    if ( xdrs->x_op == XDR_DECODE) {
	/* lookup armor type by code */
        a->type = dr1Registry_lookup( &dr1armortype, code);
	if (!a->type) return FALSE;
    }

    xdr_attr( xdrs, "damage");
    if ( !xdr_int( xdrs, &a->damage)) return FALSE;
    
    return TRUE;
}


