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
    "elf", { 0, 0, 0, 1, -1, 0 }
};

static dr1AttrAdjust race_halfling = {
    "halfling", { -1, 0, 0, 1, 0, 0 }
};

static dr1AttrAdjust race_halforc = {
    "halforc", { 1, 0, 0, 0, 1, -2 }
};

static dr1AttrAdjust race_dwarf = {
    "dwarf", { 0, 0, 0, 0, 1, -1 }
};

static dr1RegistryEntry race_entries[] = {
    { DR1R_HUMAN, &race_human },
    { DR1R_ELF, &race_elf },
    { DR1R_HOBBIT, &race_halfling },
    { DR1R_HALFORC, &race_halforc },
    { DR1R_DWARF, &race_dwarf },
    { -1, NULL }
};

dr1Registry dr1race = {
    race_entries
};

static dr1AttrAdjust sex_male = {
    "male", { 0, 0, 0, 0, 0, 0 }
};

static dr1AttrAdjust sex_female = {
    "female", { -1, 0, 0, 1, 0, 0 }
};

static dr1RegistryEntry sex_entries[] = {
    { DR1R_MALE, &sex_male },
    { DR1R_FEMALE, &sex_female },
    { -1, NULL }
};

dr1Registry dr1sex = {
    sex_entries
};

