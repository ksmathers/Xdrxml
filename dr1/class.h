#ifndef __DR1CLASS__H
#define __DR1CLASS__H

#include "registry.h"
#include "dice.h"
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
    int thac0;			/* THAC0 as a level 1 character */
} dr1ClassType;

/*-------------------------------------------------------------------
 * dr1class
 *
 *    Map of class id's to dr1ClassType objects
 */

extern dr1Registry dr1class;

#endif /* __DR1CLASS__H */


