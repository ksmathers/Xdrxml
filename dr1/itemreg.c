#include "itemreg.h"
#include "potions/heal.h"
#include "weapon.h"
#include "armor.h"

/*-------------------------------------------------------------------
 * dr1itemReg
 *
 *    This registry contains the item types known to the system.
 */
static dr1RegistryEntry e[] = {
    { (int)DR1P_HEAL, &dr1pHeal_type },
    { (int)DR1W_BASICWEAPON, &dr1Weapon_primary },
    { (int)DR1A_BASICARMOR, &dr1Armor_type },
    { -1, 0 }
};

dr1Registry dr1itemReg = {
    /* entries */ e
};

