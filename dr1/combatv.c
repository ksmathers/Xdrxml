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
int attack( dr1Player *p, int nmon, dr1Monster *m, int c, char **v) {
    int p_thac0 = dr1Player_thac0( p);
    int tohit, roll, crit;
    int mul=1;
    int i;
    dr1Monster *target;

    if (c == 1) {
	target = m;
	while (i < nmon && target->wounds >= target->hp) target++, i++;
    } else if (c == 2) {
        target = m; i=0;
	while ( i < nmon && 
	        ( target->wounds >= target->hp ||
	          strcasecmp(target->type->name, v[1])
	      )) target++, i++;
    }
    if (i >= nmon) {
	printf("Unknown monster\n");
	return -1;
    }


    /* Player swings */
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
    return 0;
}

int defend( dr1Player *p, dr1Monster *m) {
    int m_thac0 = dr1Monster_thac0( m);
    int tohit, roll, crit;
    int mul=1;
    int i;
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
	    printf("%s struck Thee! %d damage.\n", m->type->name, dam);
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
 *    Use an item while in combat
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *
 *  BUGS:
 *    FIXME: Should only be able to use items hooked to belt, or so on.
 */
int use( dr1Player *p, int c, char **v) {
    dr1Item *i;

    if (c != 2) return -1;
    i = dr1ItemSet_findName( &p->pack, v[1]);
    if (!i) return -2;

    i->type->use( p, i, /* item-function */ 0);
    return 0;
}

/*-------------------------------------------------------------------
 * dr1Combatv_showPage
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
void dr1Combatv_showPage( dr1Player *p, int nmon, dr1Monster *m) {
    int alldead;
    char cmd[80];
    char *cmds[10];
    do {
        int i;
        printf("-------------------------------------------------------\n");
	printf("Player: %-20s    Hits: %d/%d\n", p->name, HITPOINTS(p), HITPOINTSMAX(p));
	for (i=0; i < nmon; i++) {
	    printf("Monster: %-20s   Damage: %d\n", m[i].type->name, m[i].wounds);
	}
        printf("(attack, equip, use, run)\n");
	printf("Command: ");

	gets( cmd);

        i=0;
	cmds[0] = strtok( cmd, " \t\n");
	if (!cmds[0]) continue;
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

        if (!strcasecmp( cmds[0], "attack")) attack( p, nmon, m, i, cmds);
        else if (!strcasecmp( cmds[0], "run")) printf("Fleeing.\n");
        else if (!strcasecmp( cmds[0], "use")) use( p, i, cmds);
        else if (!strcasecmp( cmds[0], "equip")) equip( p, i, cmds);
	else {
	    printf("Unknown command %s.\n", cmds[0]);
	}

        alldead=1;
        for (i=0; i<nmon; i++) {
	    if (m[i].wounds < m[i].hp) { 
		alldead = 0; 
		defend( p, m); 
	    }
	}

	if (p->wounds >= p->hp) {
	    printf("Thou'rt slain.  Donai nais requiem.  Resquiat in pace.\n");
	    break;
	}

	if (alldead) {
	    int xp;
	    dr1ClassType *c;
	    dr1Money t;
	    char buf[80];
	    int gems;
	    int jewelry;
	    int *st;

	    printf("The beast is slain.  Facio Domine.\n");

            /* calculate experience w/ bonus */
	    xp = 0;
	    for (i=0; i<nmon; i++) {
		xp += m[i].type->xp + m[i].hp * m[i].type->xphp;
	    }
	    c = dr1Registry_lookup( &dr1class, p->class);
	    st = statptr( &p->base_attr, c->primaryStat);
	    if ( st && *st >= 16) xp += (xp+5)/10;
            p->xp += xp;
	    printf("Gained %d xp.\n", xp);

	    /* collect treasures */
	    for (i=0; i<nmon; i++) {
	        dr1TType_collect( m[i].type->ttype, &t, &gems, &jewelry);
		dr1Money_format( &t, buf);
		dr1Money_add( &p->purse, &t);
		printf("Collected %sfrom the %s carcass.\n", buf, m[i].type->name);
	    }

	    break;
	}
    } while (strcasecmp( cmds[0], "run")); 

}


