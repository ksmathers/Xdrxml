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

/*
 * Combat commands
 */
int attack( dr1Player *p, int nmon, dr1Monster *m, int *surprise, int c, char **v) {
    int p_thac0 = dr1Player_thac0( p);
    int tohit, roll, crit;
    int mul=1;
    int i;
    int dist;
    int nattacks;
    int tohit_penalty = 0;
    dr1Monster *target;

    /* choose target */
    if (c == 1) {
    	/* autoselect target */
	target = m; i=0;
	while (i < nmon && target->wounds >= target->hp) target++, i++;
    } else if (c == 2) {
        i = atoi(v[1]);
	if (i) { 
	    /* user selected target */
	    i--;
	    target = &m[i]; 
	} else {
	    /* autoselect target by monster type */
	    target = m; i=0;
	    while ( i < nmon && 
		    ( target->wounds >= target->hp ||
		      strcasecmp(target->type->name, v[1])
		  )) target++, i++;
	}
    }
    if (i >= nmon) {
	printf("Unknown monster\n");
	return -1;
    }

    /* check distance */
    dist = dr1Location_distance( &p->location, &m->location);
    if (p->weapon->range) {
	/* ranged combat */
        if (dist <= 10) {
	    /* too close for ranged weapon */
            tohit_penalty = 4; 
	} else {
	    /* FIXME */
	    tohit_penalty = (4 * dist) / p->weapon->range;
	}
	nattacks = p->weapon->rof;
    } else {
	/* melee combat */
	if (dist > 10) {
	    /* too far away for melee */
	    p->location = dr1Location_moveTo( &p->location, &m->location, 10);
	    dist = dr1Location_distance( &p->location, &m->location);

	    printf( "You charge the %s.  Now you are within %d feet.\n", 
		    m->type->name, dist);
	    return 0;
	} 
        nattacks = dr1Player_nattacks( p);
    }

    /* Player swings */
    *surprise = 0;
    for (i=0; i<nattacks; i++) {
	crit = 0;
	tohit = p_thac0 - target->type->ac - tohit_penalty;
	roll = dr1Dice_roll("d20");

	if ( roll == 1) {
	    /* critical miss */
	    printf("Critical Miss!\n");
	    *surprise = 1;
	    break;
	} else {
	    if ( roll == 20) {
		/* natural 20 gets a bonus */
		if ( tohit>20) roll += 5;
		else crit=dr1Dice_roll("d12");
	    } else {
		printf("Attack dice rolled %d\n", roll);
	    }
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		printf("Critical hit x2!\n");
		mul = 2;
		break;
	    case 12: 
		printf("Critical hit x3!\n");
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    /* hit */
	    int dam;

	    if (p->weapon) {
		dam = dr1Dice_roll( p->weapon->damage) + 
		    dr1Attr_damage( &p->base_attr, p->weapon->range > 0);
	    } else {
		dam = dr1Dice_roll( "d3") + 
		    dr1Attr_damage( &p->base_attr, FALSE);
	    }
	    if (p->weapon->range) {
		printf("Thunk! %d Damage.\n", dam);
	    } else {
		printf("Slash! %d Damage.\n", dam);
	    }
	    target->wounds += dam;

	} else {
	    /* missed */

	    if (p->weapon->range ) {
		printf("Thwip!\n");
	    } else {
		printf("Swish!\n");
	    }
	}
    } /* for */
    return 0;
}

int defend( dr1Player *p, dr1Monster *m, int *surprise) {
    int m_thac0 = dr1Monster_thac0( m);
    int tohit, roll, crit;
    int mul=1;
    int dist;
    int tohit_penalty = 0;
    int i;

    dist = dr1Location_distance( &p->location, &m->location);
    /* FIXME: Monsters with both ranged and melee weapons */
    if (m->type->damage[0]->range) {
	/* ranged combat */
        if (dist <= 10) {
	    /* too close for ranged weapon */
            tohit_penalty = 4; 
	} else {
	    /* FIXME */
	    tohit_penalty = (4 * dist) / p->weapon->range;
	}
    } else {
	/* melee combat */
	if (dist > 10) {
	    /* too far away for melee */
	    m->location = dr1Location_moveTo( &m->location, &p->location, 10);
	    dist = dr1Location_distance( &p->location, &m->location);

	    printf( "%s charges you.  Now you are within %d feet.\n", 
		    m->type->name, dist);
	    return 0;
	} 
    }

    /* Monster swings */
    for (i=0; i < m->type->nattacks; i++) {
	assert(m->type->damage[i]);
	tohit = m_thac0 - tohit_penalty - dr1Player_ac( p, 
		/* surprise/behind */ *surprise, 
		m->type->damage[i]->range > 0, 
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
    int dist;
    int surprise = 0;
    char cmd[80];
    char *cmds[10];
    do {
        int i;
        printf("-------------------------------------------------------\n");
	printf("Player: %-20s       Hits: %d/%d    Location: (%d,%d)\n", p->name, HITPOINTS(p), HITPOINTSMAX(p), p->location.x, p->location.y);
	for (i=0; i < nmon; i++) {
	    dist = dr1Location_distance( &p->location, &m[i].location);
	    if (m[i].wounds < m[i].hp) {
		printf("%2d:Monster: %-20s   Damage: %-3d  Range: %d\" (%d,%d)\n", i+1, m[i].type->name, m[i].wounds, dist/10, m[i].location.x, m[i].location.y );
	    } else {
		printf("%2d:Monster: %-20s   Damage: dead Range: %d\" (%d,%d)\n", i+1, m[i].type->name, dist/10, m[i].location.x, m[i].location.y );
	    }
	}
        printf("(attack, equip, use, run)\n");
	printf("Command: ");

	gets( cmd);

        i=0;
	cmds[0] = strtok( cmd, " \t\n");
	if (!cmds[0]) continue;
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

        if (!strcasecmp( cmds[0], "attack")) attack( p, nmon, m, &surprise, i, cmds);
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
		defend( p, &m[i], &surprise); 
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
	    st = dr1Attr_estatptr( &p->base_attr, c->primaryStat);
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


