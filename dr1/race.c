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

static dr1AttrAdjust race_halfogre = {
    "halfogre", { 2, -1, 0, 0, 0, -2 }
};

static dr1AttrAdjust race_dwarf = {
    "dwarf", { 1, 0, 0, 0, 1, -2 }
};

static dr1RegistryEntry race_entries[] = {
    { (int)'MAN ', &race_human },
    { (int)'ELF ', &race_elf },
    { (int)'HOBB', &race_halfling },
    { (int)'OGRE', &race_halfogre },
    { (int)'DWAR', &race_dwarf },
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
    { (int)'MALE', &sex_male },
    { (int)'FEMA', &sex_female },
    { -1, NULL }
};

dr1Registry dr1sex = {
    sex_entries
};

