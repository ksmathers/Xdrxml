#include "class.h"
#include "attr.h"


/*-------------------------------------------------------------------
 * dr1
 *
 *    These structure represent specific character classes (professions)
 *    and their initialization parameters.
 *
 *    The thac0 class column is not taken directly from D&D.  The real
 *    tables have a first 20, second 20, up to fourth 20 before having
 *    to roll 21 to hit.  I instead add 4 to a natural 20.
 */

static dr1ClassType class_mu = {
    "Magic User",
    "d4",
    "3d4*10",
    INTELLIGENCE,
    /* min_str */ 3,
    /* min_int */ 9,
    /* min_wis */ 6,
    /* min_dex */ 6,
    /* min_con */ 6,
    /* min_cha */ 6,

    21,
    5
};

static dr1ClassType class_thief = {
    "Thief",
    "d6",
    "3d6*10",
    DEXTERITY,
    /* min_str */ 6,
    /* min_int */ 6,
    /* min_wis */ 3,
    /* min_dex */ 9,
    /* min_con */ 6,
    /* min_cha */ 6,
    21,
    4
};

static dr1ClassType class_cleric = {
    "Cleric",
    "d8",
    "3d8*10",
    WISDOM,
    /* min_str */ 6,
    /* min_int */ 6,
    /* min_wis */ 9,
    /* min_dex */ 3,
    /* min_con */ 6,
    /* min_cha */ 6,
    20,
    3
};

static dr1ClassType class_fighter = {
    "Fighter",
    "d10",
    "3d10*10",
    STRENGTH,
    /* min_str */ 9,
    /* min_int */ 3,
    /* min_wis */ 6,
    /* min_dex */ 6,
    /* min_con */ 6,
    /* min_cha */ 6,
    20,
    2
};

static dr1RegistryEntry entries[] = {
    { DR1C_MU,      &class_mu },
    { DR1C_THIEF,   &class_thief },
    { DR1C_FIGHTER, &class_fighter },
    { DR1C_CLERIC,  &class_cleric },
    { -1, NULL }
};

dr1Registry dr1class = {
    entries
};

