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



/*-------------------------------------------------------------------
 * dr1MonsterType
 *
 *    The structure defines a type of monster
 */

typedef void (*dr1Monster_attack_fnp)( dr1Monster *mon, dr1Player *p);
typedef void (*dr1Monster_defend_fnp)( dr1Monster *mon, dr1Player *p);
typedef struct {
    int mtype;
    dr1Attr attr;
    dr1Dice hd;
    int ac;
    int nattacks;
    dr1Dice damage[4];
    int xp;
    int ttype;
    dr1Monster_attack_fnp attack;
    dr1Monster_defend_fnp defend;
} dr1MonsterType;

/*-------------------------------------------------------------------
 * dr1Monster
 *
 *    The structure holds a combat monster 
 */
typedef struct {
    dr1MonsterType *type
    int hp;
} dr1Monster;

/*-------------------------------------------------------------------
 * dr1monsters
 *
 *    Registry of monster types
 */
extern dr1Registry dr1monsters;

#endif /* __DR1MONSTER__H */
