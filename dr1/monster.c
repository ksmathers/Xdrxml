#include "dice.h"
#include "monster.h"
/*-------------------------------------------------------------------
 * dr1monsters
 *
 *    Registry of monster types
 */

static dr1MonsterType kobold = {
    /* mtype     */ DR1M_KOBOLD,
    /* name      */ "Kobold",
    /* attr      */ { 0, 0, 0, 0, 0, 0},
    /* hd        */ "d4+1",
    /* ac        */ 7,
    /* nattacks  */ 1,
    /* damage    */ { &dr1Weapon_halfspear, NULL, NULL, NULL },
    /* xp, xp/hp */ 5, 1,
    /* ttype     */ "J",
    /* lair      */ "OQQQQQ",
    /* attack    */ NULL,
    /* defend    */ NULL
};

static dr1RegistryEntry entries[] = {
    { DR1M_KOBOLD, &kobold },
    { -1, NULL }
};

dr1Registry dr1monsters = {
    entries
};

/*-------------------------------------------------------------------
 * dr1Monster_init
 *
 *    Initialize a monster in a buffer
 *
 *  PARAMETERS:
 *    name	Monster type name
 *    m         Monster buffer
 *
 *  RETURNS:
 *    -1 on error, 0 on success
 *
 */

int dr1Monster_init( dr1Monster *m, char *name) {
    dr1MonsterType *mtype, *tmp = NULL;
    int i;

    for (i=0; dr1monsters.entries[i].code != -1; i++) {
        tmp = dr1monsters.entries[i].value;
	if (!strcasecmp( tmp->name, name)) {
	    mtype = tmp;
	    break;
	}
    }
    if ( !mtype) return -1;

    m->type = mtype;
    m->hp = dr1Dice_roll( mtype->hd);
    m->wounds = 0;
    return 0;
}

/*-------------------------------------------------------------------
 * dr1Monster_new
 *
 *    Create a new monster object on the heap and return it.
 *
 *  PARAMETERS:
 *    name	Monster type name
 *
 *  RETURNS:
 *    monster, or NULL if the name is unknown
 *
 */

dr1Monster* dr1Monster_new( char *name) {
    dr1Monster *m;
    dr1Monster buf;

    if (dr1Monster_init( &buf, name)) return NULL;

    m = malloc( sizeof(dr1Monster));
    *m = buf;
    return m;
}

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method dr1Monster_thac0 returns the to hit for AC 0 
 *    for the monster type.
 *
 *  PARAMETERS:
 *    m     The monster in question
 *
 *  RETURNS:
 *    To hit
 *
 */

static int monster_thac0[] = {
   21, 21, 21, 20, 19, 18, 16, 15, 13, 12, 10, 9, 8, 7
};

int dr1Monster_thac0( dr1Monster *m) {
    int level;
    int thac0;
    int ndice, sides, offset, mul;
    dr1Dice_data( m->type->hd, &ndice, &sides, &offset, &mul);

    /* 
     * d4-1: 0
     * d4:   1
     * d4+1: 2
     * d8-1: 3
     * d8:   4
     * d8+1: 5
     * 2d8-3d8:  6
     * 4d8-5d8:  7
     * etc.
     *
     */
    if (ndice > 1) level = 2 + level/2;
    else level = ndice;

    if (sides >= 8) {
        level += 3;
    }

    if (ndice == 1) {
	if (offset < 0) {
	    level--;
	} else if (offset > 0) {
	    level++;
	}
    }

    if (level >= sizeof(monster_thac0) / sizeof(int)) thac0 = 7;
    else thac0 = monster_thac0[ level];

    return thac0;
}
