#ifndef __DR1GLOBALS__H
#define __DR1GLOBALS__H

/*-------------------------------------------------------------------
 *
 *    These enumerations set the boundaries for item codes of various
 *    item subtypes.
 */

enum {
    DR1ILLEGAL,
    DR1WEAPONS,
    DR1WEAPONS_END = 0x1000,
    DR1POTIONS,
    DR1POTIONS_END = 0x2000,
    DR1ARMOR,
    DR1ARMOR_END = 0x3000,
    DR1MISC,
    DR1MISC_END = 0x4000,
    DR1END
};

#endif /* __DR1GLOBALS__H */
