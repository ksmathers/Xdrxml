#ifndef __DR1CLASS__H
#define __DR1CLASS__H

#include "registry.h"
#include "dice.h"
#include "attr.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    enumerates the possible character classes
 */
enum {
    DR1C_INVALID,
    DR1C_MU,
    DR1C_CLERIC,
    DR1C_FIGHTER,
    DR1C_THIEF,
    /* not implemented below here */
    DR1C_ILLUSIONIST,
    DR1C_DRUID,
    DR1C_ASASSIN,
    DR1C_PALADIN,
    DR1C_RANGER,
    DR1C_MONK
};
    
/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

typedef struct {
    char *class;
    dr1Dice hitdice;
    dr1Dice startingMoney;
    int primaryStat;
    dr1Attr minimum;
    int thac0;			/* THAC0 as a level 1 character */
    int levelmod;		/* Levels per THAC0 adjustment */	
} dr1ClassType;

/*-------------------------------------------------------------------
 * dr1class
 *
 *    Map of class id's to dr1ClassType objects
 */

extern dr1Registry dr1class;

#endif /* __DR1CLASS__H */


