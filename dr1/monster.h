#ifndef __DR1MONSTER__H
#define __DR1MONSTER__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#ifndef __DR1ATTR__H
#   include "attr.h"
#endif

#ifndef __DR1PLAYER__H
#   include "player.h"
#endif
#ifndef __DR1REGISTRY__H
#   include "registry.h"
#endif
#ifndef __DR1WEAPON__H
#   include "weapon.h"
#endif


/*-------------------------------------------------------------------
 * dr1MonsterType
 *
 *    The structure defines a type of monster
 */

enum MonsterTypes {
    DR1M_KOBOLD
};

struct dr1Monster;
typedef void (*dr1Monster_attack_fnp)( struct dr1Monster *mon, dr1Player *p);
typedef void (*dr1Monster_defend_fnp)( struct dr1Monster *mon, dr1Player *p);
typedef struct {
    int mtype;
    char *name;
    dr1Attr attr;
    dr1Dice hd;
    int ac;
    int nattacks;
    dr1Weapon* damage[4];
    int xp, xphp;
    char *ttype;
    char *lair;
    dr1Monster_attack_fnp attack;
    dr1Monster_defend_fnp defend;
} dr1MonsterType;

/*-------------------------------------------------------------------
 * dr1Monster
 *
 *    The structure holds a combat monster 
 */
typedef struct {
    dr1MonsterType *type;
    int hp;
    int wounds;
} dr1Monster;

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

int dr1Monster_init( dr1Monster *m, char *name) ;

/*-------------------------------------------------------------------
 * dr1Monster_new
 *
 *    Create a new monster object on the heap and return it.
 *
 *  PARAMETERS:
 *    name	Monster type name
 *
 *  RETURNS:
 *    monster
 *
 */

dr1Monster* dr1Monster_new( char *name);

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

int dr1Monster_thac0( dr1Monster *m);

/*-------------------------------------------------------------------
 * dr1monsters
 *
 *    Registry of monster types
 */
extern dr1Registry dr1monsters;

#endif /* __DR1MONSTER__H */
