#ifndef __DR1RACE__H
#define __DR1RACE__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include "attr.h"
#include "registry.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

enum {
    DR1R_HUMAN,
    DR1R_ELF,
    DR1R_HOBBIT,
    DR1R_HALFOGRE,
    DR1R_DWARF
};

enum {
    DR1R_MALE,
    DR1R_FEMALE
};


/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

extern dr1Registry dr1race;
extern dr1Registry dr1sex;

#endif /* __DR1RACE__H */
