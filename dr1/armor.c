#include "registry.h"
#include "armor.h"
#include "xdrasc.h"
#include "item.h"
#include "money.h"

dr1ItemType dr1Armor_type = {
    sizeof( dr1Armor),		/* size */
    (int)'ARMO',		/* code */
    (void *)0,	 		/* use */
    (void *)0, 			/* drop */
    (void *)0, 			/* take */
    xdr_dr1Armor,		/* xdr */
};

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

dr1ArmorType dr1ArmorType_leather = {
    /* code    */ 'LEAT',
    /* name    */ "Leather",
    /* damage  */ "3d10+20",
    /* base_ac */ 9
};

static dr1RegistryEntry entries[] = {
    { 'LEAT', &dr1ArmorType_leather },
    { -1, NULL }
};

dr1Registry dr1armortype = {
    entries
};

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

dr1Armor dr1Armor_leather = {
    /* super */ {
    	/* value       */ GP(70),
	/* name        */ "Leather",
	/* encumbrance */ 100,
	/* unique      */ FALSE,
	/* inuse       */ FALSE,
	/* weapon      */ FALSE,
	/* identified  */ TRUE,
	/* uses        */ 0,
	/* type        */ &dr1Armor_type
    },
    /* type     */ &dr1ArmorType_leather,
    /* damage   */ 0
};


/*-------------------------------------------------------------------
 * dr1Armor_new
 *
 *    The method new initializes a new armor
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

dr1Armor* dr1Armor_new( char *name) {
    dr1Armor *a = malloc( sizeof(dr1Armor));
    *a = dr1Armor_leather;
    a->damage = dr1Dice_roll( a->type->damage);
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

bool_t xdr_dr1Armor( XDR *xdrs, dr1Item *i) {
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


