#include "class.h"

static dr1ClassType class_mu = {
    "Magic User",
    "d4",
    "3d4*10",
    'INT ',
    25
};

static dr1ClassType class_thief = {
    "Thief",
    "d6",
    "3d6*10",
    'DEX ',
    23
};

static dr1ClassType class_cleric = {
    "Cleric",
    "d8",
    "3d8*10",
    'WIS ',
    22
};

static dr1ClassType class_fighter = {
    "Fighter",
    "d10",
    "3d10*10",
    'STR ',
    20
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

