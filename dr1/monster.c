#include "monster.h"
/*-------------------------------------------------------------------
 * dr1monsters
 *
 *    Registry of monster types
 */

static dr1MonsterType kobold = {
    /* mtype     */ 'KOBO', 
    /* name      */ "Kobold",
    /* attr      */ { 0, 0, 0, 0, 0, 0},
    /* hd        */ "d4+1",
    /* ac        */ 7,
    /* nattacks  */ 1,
    /* damage    */ { &dr1Weapon_halfspear, NULL, NULL, NULL },
    /* xp        */ 10,
    /* ttype     */ 'J',
    /* attack    */ NULL,
    /* defend    */ NULL
};

static dr1RegistryEntry entries[] = {
    { 'KOBO', &kobold },
    { -1, NULL }
};

dr1Registry dr1monsters = {
    entries
};

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
    dr1MonsterType *mtype = NULL;
    dr1Monster *m;
    int i;

    for (i=0; dr1monsters.entries[i].code != -1; i++) {
        mtype = dr1monsters.entries[i].value;
	if (!strcasecmp( mtype->name, name)) {
	    break;
	}
    }
    if (!mtype) return NULL;

    m = malloc( sizeof(dr1Monster));
    m->type = mtype;
    m->hp = dr1Dice_roll( mtype->hd);
    m->full_hp = m->hp;
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

int dr1Monster_thac0( dr1Monster *m) {
    int level;
    int ndice, sides, offset, mul;
    dr1Dice_data( &ndice, &sides, &offset, &mul);
    level = ndice;
    if (sides > 4) {
	level *= 2;
    }
    if (offset < 0) {
	level--;
    }
    return 21 - level;
}
