#include <stdlib.h>
#include "attr.h"
#include "dice.h"

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
    bool_t res;
    res = xdr_int( xdrs, &a->_str);
    res |= xdr_int( xdrs, &a->_int);
    res |= xdr_int( xdrs, &a->_wis);
    res |= xdr_int( xdrs, &a->_dex);
    res |= xdr_int( xdrs, &a->_con);
    res |= xdr_int( xdrs, &a->_cha);
    return res;
}

