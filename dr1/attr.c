#include <stdlib.h>
#include "attr.h"
#include "dice.h"
#include "xdrasc.h"

/*-------------------------------------------------------------------
 * Strength modifiers
 *
 *   dr1Attr_str_damage   Damage adjustment
 *   dr1Attr_str_weight   Weight adjustment
 */

static int str_damage[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25*/
    { -4,-3,-2,-1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12};

static int str_weight[] = 
    /*   3    4    5    6    7   8   9  10  11  12  13  14  15  16  17  18 */
    { -600,-400,-300,-200,-100,  0,  0,  0,  0,  0,100,200,300,400,500,600 };

/*-------------------------------------------------------------------
 * Constitution modifiers
 *
 *   dr1Attr_str_damage   Damage adjustment
 *   dr1Attr_str_weight   Weight adjustment
 */

static int con_hp[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25*/
    { -4,-3,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5};

/*-------------------------------------------------------------------
 * dr1Attr_damage
 *
 *    Returns the attributes adjustment for damage
 *
 * Parameters:
 *    a       Attributes of the player being checked
 *    ranged  Flag that is TRUE if a ranged weapon is being used
 *
 * Returns:
 *    Attribute value
 */

int dr1Attr_damage( dr1Attr *a, int ranged) {
    int str;
    if (ranged) return 0;
    str = a->_str;

    if (str > 25) str=25;
    if (str < 3) str=3;
    return str_damage[ str - 3];
}

/*-------------------------------------------------------------------
 * dr1Attr_hp
 *
 *    Returns the attributes adjustment for hit points
 *
 * Parameters:
 *    a       Attributes of the player being checked
 *    fighter Flag that is TRUE if the player is a Fighter class
 *
 * Returns:
 *    Attribute value
 */

int dr1Attr_hp( dr1Attr *a, int fighter) {
    int con, hpadj;
    con = a->_con;

    if (con > 25) con=25;
    if (con < 3) con=3;

    hpadj = con_hp[ con-3]; 
    if (hpadj > 2 && !fighter) {
    	hpadj -= 2;
	if (hpadj == 1) hpadj++;
    }
    return hpadj;
}

/*-------------------------------------------------------------------
 * dr1Attr_gen_mode4
 *
 *    Generates a new Attribute value by the fourth method; roll
 *    4d6 and keep the best 3.
 *
 * Returns:
 *    Attribute value
 */

static int dr1Attr_gen_mode4( void)
{
    int r, sum=0, min=6;
    int i;
    for ( i=0; i<4; i++) {
        r = dr1Dice_roll("d6");
	sum += r;
	if (r < min) min = r;
    }
    sum -= r;
    return sum;
}

/*-------------------------------------------------------------------
 * dr1Attr_gen_mode1
 *
 *    Generates a new Attribute value by the first method; roll
 *    3d6
 *
 * Returns:
 *    Attribute value
 */
static int dr1Attr_gen_mode1( void)
{
    int r;
    r = dr1Dice_roll("3d6");
    return r;
}

/*-------------------------------------------------------------------
 * dr1Attr_enum
 *
 *    Get a pointer to the N-th Attribute
 *
 * Returns
 *    Pointer to the Attribute
 */
static int *dr1Attr_enum( dr1Attr *attr, int n) {
    switch( n) {
	case 0:
	    return &attr->_str;
	case 1:
	    return &attr->_int;
	case 2:
	    return &attr->_wis;
	case 3:
	    return &attr->_dex;
	case 4:
	    return &attr->_con;
	case 5:
	    return &attr->_cha;
    }
    return NULL;
}

/*-------------------------------------------------------------------
 * dr1Attr_create_mode1
 *
 *    Generates a new Attribute value by the first method; roll
 *    3d6, six times, in order.
 *
 * Returns:
 *    Attribute value
 */
dr1Attr dr1Attr_create_mode1( void) {
    dr1Attr p;
    int i;

    for (i=0; i<6; i++) {
	*(dr1Attr_enum( &p, i)) = dr1Attr_gen_mode1();
    }
    return p;
}

/*-------------------------------------------------------------------
 * dr1Attr_create_mode4
 *
 *    Generates a new Attribute value by the first method; roll
 *    4d6, keep best three, six times, in order.
 *
 * Returns:
 *    Attribute value
 */
dr1Attr dr1Attr_create_mode4( void) {
    dr1Attr p;

    int i;

    for (i=0; i<6; i++) {
	*(dr1Attr_enum(&p, i)) = dr1Attr_gen_mode4();
    }
    return p;
}



/*-------------------------------------------------------------------
 * xdr_dr1Attr( xdrs, dr1Attr*)
 */
bool_t xdr_dr1Attr( XDR *xdrs, dr1Attr *a) {
    xdr_attr( xdrs, "_str");
    if (!xdr_int( xdrs, &a->_str)) return FALSE;

    xdr_attr( xdrs, "_int");
    if (!xdr_int( xdrs, &a->_int)) return FALSE;

    xdr_attr( xdrs, "_wis");
    if (!xdr_int( xdrs, &a->_wis)) return FALSE;

    xdr_attr( xdrs, "_dex");
    if (!xdr_int( xdrs, &a->_dex)) return FALSE;

    xdr_attr( xdrs, "_con");
    if (!xdr_int( xdrs, &a->_con)) return FALSE;

    xdr_attr( xdrs, "_cha");
    if (!xdr_int( xdrs, &a->_cha)) return FALSE;
    return TRUE;
}

/*-------------------------------------------------------------------
 * dr1Attr_adjust 
 *
 *    Adds two attribute structs
 *
 * Parameters:
 *    a    Attribute set to adjust
 *    b    Amount to adjust by
 *    mul  Multiplier
 *
 * Side effects:
 *    b is added into a
 */
void dr1Attr_adjust( dr1Attr *a, dr1Attr *b, int mul) {
    a->_str += b->_str * mul;
    a->_int += b->_int * mul;
    a->_wis += b->_wis * mul;
    a->_dex += b->_dex * mul;
    a->_con += b->_con * mul;
    a->_cha += b->_cha * mul;
}

