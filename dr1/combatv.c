#include <string.h>
#include <assert.h>
#include "combatv.h"
#include "player.h"
#include "monster.h"
#include "dice.h"
#include "class.h"
#include "ttype.h"

/*
 * Utility functions
 */
static int *statptr( dr1Attr *a, int stat) {
    if ( stat == STRENGTH) return &a->_str;
    if ( stat == INTELLIGENCE) return &a->_int;
    if ( stat == WISDOM) return &a->_wis;
    if ( stat == DEXTERITY) return &a->_dex;
    if ( stat == CONSTITUTION) return &a->_con;
    if ( stat == CHARISMA) return &a->_cha;
    return NULL;
}

/*
 * Combat commands
 */
int attack( dr1Player *p, dr1Monster *m, int c, char **v) {
    int fleeing = 0;
    int p_thac0 = dr1Player_thac0( p);
    int m_thac0 = dr1Monster_thac0( m);
    int tohit, roll, crit;
    int mul=1;
    int i;

    if (!c) return -1;
    if (!strcasecmp( v[0], "run")) {
	fleeing = 1;
    }


    /* Player swings */
    if (!fleeing) {
	crit = 0;
	tohit = p_thac0 - m->type->ac;
	roll = dr1Dice_roll("d20");
	if ( roll == 20) {
	    /* natural 20 gets a bonus */
	    if ( tohit>20) roll += 5;
	    else crit=dr1Dice_roll("d12");
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		mul = 2;
		break;
	    case 12: 
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    int dam;

            if (p->weapon) {
		dam = dr1Dice_roll( p->weapon->damage) + 
		    dr1Attr_damage( &p->base_attr, p->weapon->ranged);
	    } else {
		dam = dr1Dice_roll( "d3") + 
		    dr1Attr_damage( &p->base_attr, FALSE);
	    }
	    printf("Hit! %d Damage.\n", dam);
	    m->wounds += dam;
	} else {
	    printf("Swish!\n");
	}
    } /* if fleeing */

    /* Monster swings */
    for (i=0; i < m->type->nattacks; i++) {
	assert(m->type->damage[i]);
	/* FIXME */
	tohit = m_thac0 - dr1Player_ac( p, 
		/* surprise/behind */ 0, 
		m->type->damage[i]->ranged, 
		m->type->damage[i]->dtype
	    );
	roll = dr1Dice_roll("d20");
	if ( roll == 20) {
	    /* natural 20 gets a bonus */
	    if ( tohit>20) roll += 4;
	    else crit=dr1Dice_roll("d12");
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		mul = 2;
		break;
	    case 12: 
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    int dam;
	    dam = dr1Dice_roll( m->type->damage[i]->damage);
	    printf("Struck Thee! %d damage.\n", dam);
	    p->wounds += dam;
	} else {
	    printf("Swish!\n");
	}
    } /* for */

    return 0;
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
void dr1Combatv_showPage( dr1Player *p, dr1Monster *m) {
    char cmd[80];
    char *cmds[10];
    do {
        int i;
        printf("-------------------------------------------------------\n");
	printf("Player: %-20s    Hits: %d/%d\n", p->name, HITPOINTS(p), HITPOINTSMAX(p));
	printf("Monster: %-20s   Damage: %d\n", m->type->name, m->wounds);
        printf("(attack, equip, run)\n");
	printf("Command: ");

	gets( cmd);

        i=0;
	cmds[0] = strtok( cmd, " \t\n");
	if (!cmds[0]) continue;
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

        if (!strcasecmp( cmds[0], "attack")) attack( p, m, i, cmds);
        else if (!strcasecmp( cmds[0], "run")) attack( p, m, i, cmds);
        else if (!strcasecmp( cmds[0], "equip")) equip( p, i, cmds);
	else {
	    printf("Unknown command %s.\n", cmds[0]);
	}

	if (p->wounds > p->hp) {
	    printf("Thou'rt slain.  Donai nais requiem.  Resquiat in pace.\n");
	    break;
	}

	if (m->wounds > m->hp) {
	    int xp;
	    dr1ClassType *c;
	    dr1Money t;
	    char buf[80];
	    int gems;
	    int jewelry;
	    int *st;

	    printf("The beast is slain.  Facio Domine.\n");

            /* calculate experience w/ bonus */
	    xp = m->type->xp + m->hp * m->type->xphp;
	    c = dr1Registry_lookup( &dr1class, p->class);
	    st = statptr( &p->base_attr, c->primaryStat);
	    if ( st && *st >= 16) xp += (xp+5)/10;
            p->xp += xp;
	    printf("Gained %d xp.\n", xp);

	    /* collect treasures */
	    dr1TType_collect( m->type->ttype, &t, &gems, &jewelry);
	    dr1Money_format( &t, buf);
	    dr1Money_add( &p->purse, &t);
	    printf("Collected %sfrom the carcass.\n", buf);

	    break;
	}
    } while (strcasecmp( cmds[0], "run")); 

}


