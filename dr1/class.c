#include "class.h"

static dr1ClassType class_mu = {
    "Magic User",
    "d4",
    "3d4*10",
    'INT '
};

static dr1ClassType class_thief = {
    "Thief",
    "d6",
    "3d6*10",
    'DEX '
};

static dr1ClassType class_cleric = {
    "Cleric",
    "d8",
    "3d8*10",
    'WIS '
};

static dr1ClassType class_fighter = {
    "Fighter",
    "d10",
    "3d10*10",
    'STR '
};

static dr1RegistryEntry entries[] = {
    { 'MU  ', &class_mu },
    { 'THIE', &class_thief },
    { 'FIGH', &class_fighter },
    { 'CLER', &class_cleric },
    { -1, NULL }
};

dr1Registry dr1class = {
    entries
};

