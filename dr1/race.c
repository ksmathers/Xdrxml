#include "race.h"
#include "registry.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

static dr1AttrAdjust race_human = {
    "human", { 0, 0, 0, 0, 0, 0 }
};

static dr1AttrAdjust race_elf = {
    "elf", { 0, 0, 0, 1, -2, 1 }
};

static dr1AttrAdjust race_halfling = {
    "halfling", { -1, 0, 0, 2, 0, -1 }
};

static dr1RegistryEntry entries[] = {
    { (int)'MAN ', &race_human },
    { (int)'ELF ', &race_elf },
    { (int)'HOBB', &race_halfling },
    { -1, NULL }
};

dr1Registry dr1race = {
    entries
};

