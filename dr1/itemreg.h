#ifndef __DR1ITEMREG__H
#define __DR1ITEMREG__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "registry.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    This registry contains the item types known to the system.
 */
extern dr1Registry dr1itemReg;

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method dr1Registry_lookup returns the class pointed to
 *    by a class type code.
 *
 *  PARAMETERS:
 *    r     The registry to search
 *    code  The serial id for the class
 *
 *  RETURNS:
 *    Pointer to the class
 *
 */

void *dr1Registry_lookup( dr1Registry *r, int code);


#endif /* __DR1ITEMREG__H */
