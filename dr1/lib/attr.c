#include <stdlib.h>
#include "dr1.h"
#include "attr.h"
#include "dice.h"

/*-------------------------------------------------------------------
 * attribute
 *
 *   Convert from enum Attribute to string
 */

char *dr1attribute[] = {
    "Strength", "Intelligence", "Wisdom", 
    "Dexterity", "Constitution", "Charisma"
};

/*-------------------------------------------------------------------
 * Strength modifiers
 *
 *   dr1Attr_str_damage   Damage adjustment
 *   dr1Attr_str_weight   Weight adjustment
 */

static int str_damage[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 */
    { -1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 7, 8, 9,10,11,12 };

static int estr_damage[] = 
    /* 01-50  51-75  76-90  91-99  00 */
    {    3,     3,     4,     5,    6 };

#define EXCEPTIONAL_IDX(x) ((x)<51?0:((x)<76?1:((x)<91?2:((x)<100?3:4))))

static int str_weight[] = 
 /* 3   4   5   6   7 8 9 10 11 12 13 14 15 16 17 18 19  20  21  22  23  24 */
 {-35,-25,-25,-15,-15,0,0, 0, 0,10,10,20,20,35,50,75,450,500,600,750,900,1200};

static int estr_weight[] = 
    /* 01-50 51-75 76-90 91-99   00 */
    {    100,  125,  150,  200, 300 };

static int str_force[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 */
    { -1,-1, 0, 0, 0, 1, 1, 2, 2, 4, 4, 7, 7,10,13,16,50,60,70,80,90,100 };

static int str_tohit[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 */
    { -3,-2,-2,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 3, 4, 4, 5, 6 };

static int estr_tohit[] = 
    /* 01-50 51-75 76-90 91-99   00 */
    {      1,    2,    2,    2,   3 };

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
 * Dexterity modifiers
 *
 *   dr1Attr_dex_ac       AC adjustment
 *   dr1Attr_dex_tohit    ToHit adjustment (ranged only)
 */

static int dex_ac[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25*/
    { -4,-3,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5};

static int dex_tohit[] = 
    /* 3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25*/
    { -3,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5};

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
    int damadj;
    if (ranged) return 0;
    str = a->_str;

    if (str > 25) str=25;
    if (str < 3) str=3;
    if (str == 18 && a->estr) {
	damadj = estr_damage[ EXCEPTIONAL_IDX( a->estr)];
    } else {
	damadj = str_damage[ str - 3];
    }
    return damadj;
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
 * dr1Attr_ac
 *
 *    Returns the attributes adjustment AC
 *
 * Parameters:
 *    a       Attributes of the player being checked
 *
 * Returns:
 *    Attribute value
 */

int dr1Attr_ac( dr1Attr *a, int surprise) {
    int dex, acadj;

    if (surprise) return 0;
    dex = a->_dex;

    if (dex > 25) dex=25;
    if (dex < 3) dex=3;

    acadj = dex_ac[ dex-3]; 
    return acadj;
}

/*-------------------------------------------------------------------
 * dr1Attr_tohit
 *
 *    Returns the attributes adjustment for tohit
 *
 * Parameters:
 *    a       Attributes of the player being checked
 *
 * Returns:
 *    Attribute value
 */

int dr1Attr_tohit( dr1Attr *a, int ranged) {
    int str, dex, tohit;
    dex = a->_dex;
    str = a->_str;
    if (ranged) {
	/* ranged weapons get dex bonus */
	if (dex > 25) dex=25;
	if (dex < 3) dex=3;
	tohit = dex_tohit[ dex - 3];
    } else {
	/* melee weapons get str bonus */
	if (str > 25) str=25;
	if (str < 3) str=3;
	if (str == 18 && a->estr) {
	    tohit = estr_tohit[ EXCEPTIONAL_IDX( a->estr)];
	} else {
	    tohit = str_tohit[ str - 3];
	}
    }

    return tohit;
}

/*-------------------------------------------------------------------
 * dr1Attr_estatptr
 *
 *    Get pointer to attribute structure element from the 
 *    attribute enum
 *
 * Parameters:
 *    a    Attribute set
 *    stat Attribute element to locate
 *
 * Returns:
 *    Pointer to attribute element (e.g: strength)
 */
int *dr1Attr_estatptr( dr1Attr *a, enum Attribute stat) {
    if ( stat == STRENGTH) return &a->_str;
    if ( stat == INTELLIGENCE) return &a->_int;
    if ( stat == WISDOM) return &a->_wis;
    if ( stat == DEXTERITY) return &a->_dex;
    if ( stat == CONSTITUTION) return &a->_con;
    if ( stat == CHARISMA) return &a->_cha;
    return NULL;
}

/*-------------------------------------------------------------------
 * dr1Attr_statptr
 *
 *    Get pointer to attribute structure element from the 
 *    attribute name
 *
 * Parameters:
 *    a    Attribute set
 *    s    Name of attribute to locate
 *
 * Returns:
 *    Pointer to attribute element (e.g: strength)
 */
int *dr1Attr_statptr( dr1Attr *a, char *s) {
    if ( !strcasecmp( s, "str")) return &a->_str;
    if ( !strcasecmp( s, "int")) return &a->_int;
    if ( !strcasecmp( s, "wis")) return &a->_wis;
    if ( !strcasecmp( s, "dex")) return &a->_dex;
    if ( !strcasecmp( s, "con")) return &a->_con;
    if ( !strcasecmp( s, "cha")) return &a->_cha;
    printf("Unknown attribute '%s'\n", s);
    return NULL;
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

    p.estr = 0;
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

    xdr_attr( xdrs, "estr");
    if (!xdr_int( xdrs, &a->estr)) return FALSE;

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
void dr1Attr_adjust( dr1Attr *a, dr1AttrAdjust *b, int mul) {
    a->_str += b->offset._str * mul;
    a->_int += b->offset._int * mul;
    a->_wis += b->offset._wis * mul;
    a->_dex += b->offset._dex * mul;
    a->_con += b->offset._con * mul;
    a->_cha += b->offset._cha * mul;
}

