#ifndef __DR1CLASS__H
#define __DR1CLASS__H

#include "registry.h"
#include "dice.h"

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
    DR1C_THIEF
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
    int min_str;
    int min_int;
    int min_wis;
    int min_dex;
    int min_con;
    int min_cha;
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


