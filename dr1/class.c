#include "class.h"


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
    'INT ',
    21,
    5
};

static dr1ClassType class_thief = {
    "Thief",
    "d6",
    "3d6*10",
    'DEX ',
    21,
    4
};

static dr1ClassType class_cleric = {
    "Cleric",
    "d8",
    "3d8*10",
    'WIS ',
    20,
    3
};

static dr1ClassType class_fighter = {
    "Fighter",
    "d10",
    "3d10*10",
    'STR ',
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

