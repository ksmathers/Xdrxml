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

typedef struct {
    int _str;
    int _int;
    int _wis;
    int _dex;
    int _con;
    int _cha;
} dr1Attr;

/*-------------------------------------------------------------------
 * xdr_dr1Attr( xdrs, dr1Attr*)
 */
bool_t xdr_dr1Attr( XDR *xdrs, dr1Attr*);


#endif /* __DR1ATTR__H */
