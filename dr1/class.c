#include "class.h"

static dr1ClassType class_mu = {
    "Magic User",
    "d4",
    "3d4*10",
    'INT '
};

static dr1RegistryEntry entries[] = {
    { (int)'MU  ', &class_mu },
#if 0
    { (int)'THIE', &class_thief },
    { (int)'FIGH', &class_fighter },
    { (int)'CLER', &class_cleric },
#endif
    { -1, NULL }
};

dr1Registry dr1class = {
    entries
};

