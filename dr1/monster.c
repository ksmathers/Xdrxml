#include "monster.h"
/*-------------------------------------------------------------------
 * dr1monsters
 *
 *    Registry of monster types
 */

static dr1MonsterType mkobold = {
    /* mtype     */ 'MKOB', 
    /* attr      */ {},
    /* hd        */ "d4+1",
    /* ac        */ 15,
    /* nattacks  */ 2,
    /* damage    */ { 'LXBO', 'HSPR' },
    /* xp        */ 10,
    /* ttype     */ 'J',
    /* attack    */ NULL,
    /* defend    */ NULL
}

static dr1Registry dr1monsters[] = {
    { 'KOBO', &mkobold },
    { -1, NULL }
}

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

