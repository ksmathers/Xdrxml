#ifndef __DR1ATTR__H
#define __DR1ATTR__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1Attr
 *
 *    Player and monster attributes pack.  These are the six basic
 *    attributes: Strength, Intelligence, Wisdom, Dexterity, Constitution
 *    and Charisma.
 */

enum Attribute {
    STRENGTH, INTELLIGENCE, WISDOM, DEXTERITY, CONSTITUTION, CHARISMA
};
extern char *attribute[];

/*-------------------------------------------------------------------
 * dr1Attr
 *
 *    Player and monster attributes pack.  These are the six basic
 *    attributes: Strength, Intelligence, Wisdom, Dexterity, Constitution
 *    and Charisma.
 */

typedef struct {
    int _str;
    int _int;
    int _wis;
    int _dex;
    int _con;
    int _cha;
    int estr;
} dr1Attr;

typedef struct {
    char *type;
    dr1Attr offset;
} dr1AttrAdjust;

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

int dr1Attr_damage( dr1Attr *a, int ranged);

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

int dr1Attr_hp( dr1Attr *a, int fighter);

/*-------------------------------------------------------------------
 * dr1Attr_estatptr
 *
 *    Get pointer to attribute structure element from the 
 *    attribute enumeration
 *
 * Parameters:
 *    a    Attribute set
 *    s    Attribute to locate
 *
 * Returns:
 *    Pointer to attribute element (e.g: strength)
 */
int *dr1Attr_estatptr( dr1Attr *a, enum Attribute s);

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
int *dr1Attr_statptr( dr1Attr *a, char *s);

/*-------------------------------------------------------------------
 * dr1Attr_create_mode1
 *
 *    Generates a new Attribute value by the first method; roll
 *    3d6, six times, in order.
 *
 * Returns:
 *    Attribute value
 */
dr1Attr dr1Attr_create_mode1( void);

/*-------------------------------------------------------------------
 * dr1Attr_create_mode4
 *
 *    Generates a new Attribute value by the first method; roll
 *    4d6, keep best three, six times, in order.
 *
 * Returns:
 *    Attribute value
 */
dr1Attr dr1Attr_create_mode4( void);


/*-------------------------------------------------------------------
 * dr1Attr_adjust 
 *
 *    Adds two attribute structs
 *
 * Parameters:
 *    a    Attribute set to adjust
 *    b    Amount to adjust by
 *    mul  -1 - subtract
 *          1 - add
 *
 * Side effects:
 *    b is added into a
 */
void dr1Attr_adjust( dr1Attr *a, dr1AttrAdjust *b, int mul);

/*-------------------------------------------------------------------
 * xdr_dr1Attr( xdrs, dr1Attr*)
 */
bool_t xdr_dr1Attr( XDR *xdrs, dr1Attr*);


#endif /* __DR1ATTR__H */
