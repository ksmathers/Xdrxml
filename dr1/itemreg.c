#include "itemreg.h"
#include "potions/heal.h"

/*-------------------------------------------------------------------
 * dr1itemReg
 *
 *    This registry contains the item types known to the system.
 */
static dr1RegistryEntry e[] = {
    { (int)'PHEA', &dr1pHeal_type },
    { -1, 0 }
};

dr1Registry dr1itemReg = {
    /* entries */ e
};

