#include <string.h>
#include <assert.h>
#include <errno.h>
#include "combatv.h"
#include "player.h"
#include "monster.h"
#include "dice.h"
#include "class.h"
#include "ttype.h"
#include "qio.h"

/*
 * Utility functions
 */

/*
 * Combat commands
 */
int attack( dr1Context *ctx, int nmon, dr1Monster *m, int *surprise, int c, char **v) {
    dr1Player *p = &ctx->player;
    int p_thac0 = dr1Player_thac0( p);
    int tohit, roll, crit;
    int mul=1;
    int i;
    int dist;
    int nattacks;
    int tohit_penalty = 0;
    int damage_bonus = 0;
    dr1Monster *target;
    dr1Weapon *missile = NULL;

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
	qprintf( ctx, "Unknown monster\n");
	return -1;
    }

    /* check attributes */
    if (p->weapon->min_str > p->curr_attr._str) {
	qprintf( ctx, "You aren't strong enough to wield a %s.\n", 
		p->weapon->super.name);
	return -5;
    }

    /* check distance */
    dist = dr1Location_distance( &p->location, &target->location);
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

	if (p->weapon->missile) {
	    missile = (dr1Weapon*)p->gauche;
	    if (!missile) {
		qprintf( ctx, "No arrows ready.\n");
		return -1;
	    }
            if (missile->missile != p->weapon->missile) {
		qprintf( ctx, "You can't fire a %s with a %s\n", 
			missile->super.name,
			p->weapon->super.name
		    );
		return -2;
	    }
	}
    } else {
	/* melee combat */
	if (dist > 10) {
	    /* too far away for melee */
	    p->location = dr1Location_moveTo( &p->location, &target->location, 10);
	    dist = dr1Location_distance( &p->location, &target->location);

	    qprintf( ctx,  "You charge the %s.  Now you are within %d feet.\n", 
		    target->type->name, dist);
	    return 0;
	} 
        nattacks = dr1Player_nattacks( p);
    }
    tohit_penalty -= p->weapon->plusToHit;
    damage_bonus = p->weapon->plusToDamage;

    /* Player swings */
    *surprise = 0;
    for (i=0; i<nattacks; i++) {
	crit = 0;
	tohit = p_thac0 - target->type->ac - tohit_penalty;
	roll = dr1Dice_roll("d20");
	if ( p->weapon->missile) {
	    /* weapon uses missiles */

	    if (!missile) {
		qprintf( ctx, "Out of arrows.\n");
		return -1;
	    }

	    if (missile->super.count <= 0) {
		p->gauche = NULL;
		/* FIXME: free missile */
		missile = NULL;
		return -2;
	    }

            missile->super.count --;
	}

	if ( roll == 1) {
	    /* critical miss */
	    qprintf( ctx, "Critical Miss!\n");
	    *surprise = 1;
	    break;
	} else {
	    if ( roll == 20) {
		/* natural 20 gets a bonus */
		qprintf( ctx, "Rolled natural 20\n");
		if ( tohit>20) roll += 5;
		else crit=dr1Dice_roll("d12");
	    } else {
		qprintf( ctx, "Attack dice rolled %d\n", roll);
	    }
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		qprintf( ctx, "Critical hit x2!\n");
		mul = 2;
		break;
	    case 12: 
		qprintf( ctx, "Critical hit x3!\n");
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    /* hit */
	    int dam;

	    if (p->weapon) {
		dam = dr1Dice_roll( p->weapon->damage) + 
		    dr1Attr_damage( &p->base_attr, p->weapon->range > 0);
/*	        printf( "Damage %s + str = %d", p->weapon->damage, dam); /**/
	    } else {
		dam = dr1Dice_roll( "d3") + 
		    dr1Attr_damage( &p->base_attr, FALSE);
	    }

	    if (missile) {
		dam += dr1Dice_roll( missile->damage);
/*	        printf( "+ Missile Damage %s\n", missile->damage); /**/
	    }

	    dam += damage_bonus;
	    dam *= mul;
	    if (p->weapon->range) {
		qprintf( ctx, "Thunk! %d Damage.\n", dam);
	    } else {
		qprintf( ctx, "Slash! %d Damage.\n", dam);
	    }
	    target->wounds += dam;

	} else {
	    /* missed */

	    if (p->weapon->range ) {
		qprintf( ctx, "Thwip!\n");
	    } else {
		qprintf( ctx, "Swish!\n");
	    }
	}
    } /* for */
    return 0;
}

int defend( dr1Context *ctx, dr1Monster *m, int *surprise) {
    dr1Player *p = &ctx->player;
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

	    qprintf( ctx,  "%s charges you.  Now you are within %d feet.\n", 
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
	    qprintf( ctx, "%s struck Thee! %d damage.\n", m->type->name, dam);
	    p->wounds += dam;
	} else {
	    qprintf( ctx, "Swish!\n");
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
struct combatargs_t {
    int nmon;
    dr1Monster *m;
};

int dr1Combatv_showPage( dr1Context *ctx) {
    struct combatargs_t *args = dr1Context_args( ctx, "combatargs_t");
    struct {
	int state;
	char cmd[80];
	char *cmds[10];
	int surprise;
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    int nmon = args->nmon;
    dr1Player *p = &ctx->player;
    dr1Monster *m = args->m;
    int alldead;
    int dist;
    int i;
    enum { SHOWPAGE, PROMPT, GETCMD, ALLDEAD };
    
    switch ( autos->state) {
	case SHOWPAGE:
	    qprintf( ctx, "-------------------------------------------------------\n");
	    qprintf( ctx, "Player: %-20s       Hits: %d/%d    Location: (%d,%d)\n", p->name, HITPOINTS(p), HITPOINTSMAX(p), p->location.x, p->location.y);
	    for (i=0; i < nmon; i++) {
		dist = dr1Location_distance( &p->location, &m[i].location);
		if (m[i].wounds < m[i].hp) {
		    qprintf( ctx, "%2d:Monster: %-20s   Damage: %-3d  Range: %d\" (%d,%d)\n", i+1, m[i].type->name, m[i].wounds, dist/10, m[i].location.x, m[i].location.y );
		} else {
		    qprintf( ctx, "%2d:Monster: %-20s   Damage: dead Range: %d\" (%d,%d)\n", i+1, m[i].type->name, dist/10, m[i].location.x, m[i].location.y );
		}
	    }

	case PROMPT:
	    qprintf( ctx, "(attack, equip, use, run)\n");
	    qprintf( ctx, "Command: ");
	    autos->state = GETCMD;

	case GETCMD:
	    if (qgets( autos->cmd, sizeof(autos->cmd), ctx)) return 1;

	    /* tokenize command */
	    i=0;
	    autos->cmds[0] = strtok( autos->cmd, " \t\n");
	    if (!autos->cmds[0]) {
		autos->state = PROMPT;
		return 0;
	    }
	    while ((autos->cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

	    /* interpret command */
	    if (!strcasecmp( autos->cmds[0], "attack")) {
		attack( ctx, nmon, m, &autos->surprise, i, autos->cmds);
	    } else if (!strcasecmp( autos->cmds[0], "run")) {
		qprintf( ctx, "Fleeing.\n");
		dr1Context_popcall( ctx, 0);
		return 0;
	    } else if (!strcasecmp( autos->cmds[0], "use")) {
		use( p, i, autos->cmds);
/*            } else if (!strcasecmp( autos->cmds[0], "equip")) {
		equip( p, i, autos->cmds); /**/
	    } else {
		qprintf( ctx, "Unknown command %s.\n", autos->cmds[0]);
		autos->state = PROMPT;
		return 0;
	    }

	    alldead=1;
	    for (i=0; i<nmon; i++) {
		if (m[i].wounds < m[i].hp) { 
		    alldead = 0; 
		    defend( ctx, &m[i], &autos->surprise); 
		}
	    }

	    if (HITPOINTS(p) <= 0) {
		qprintf( ctx, "Thou'rt slain.  Dux vitae mortuus, regnat vivus.\n");
		dr1Context_popcall( ctx, 0);
		return 0;
	    }

	    if (alldead) {
		autos->state = ALLDEAD;
	    } else {
		autos->state = SHOWPAGE;
	    }
	    return 0;

	case ALLDEAD:
	    {
		int xp;
		dr1ClassType *c;
		dr1Money t;
		char buf[80];
		int gems;
		int jewelry;
		int *st;

		qprintf( ctx, "The beast is slain.  Facio Domine.\n");

		/* calculate experience w/ bonus */
		xp = 0;
		for (i=0; i<nmon; i++) {
		    xp += m[i].type->xp + m[i].hp * m[i].type->xphp;
		}
		c = dr1Registry_lookup( &dr1class, p->class);
		st = dr1Attr_estatptr( &p->base_attr, c->primaryStat);
		if ( st && *st >= 16) xp += (xp+5)/10;
		p->xp += xp;
		qprintf( ctx, "Gained %d xp.\n", xp);

		/* collect treasures */
		for (i=0; i<nmon; i++) {
		    dr1TType_collect( m[i].type->ttype, &t, &gems, &jewelry);
		    dr1Money_format( &t, buf);
		    dr1Money_add( &p->purse, &t);
		    qprintf( ctx, "Collected %sfrom the %s carcass.\n", buf, m[i].type->name);
		}
	    }
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */

    errno = ENOSYS;
    return -1;

}


int dr1Combatv_showDialog( dr1Context *ctx) {
    dr1Player *p = &ctx->player;
    struct {
	int state;
	dr1Monster m[10];
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    char mname[30];
    dr1Monster *m = autos->m;
    int nmon;
    int dead;
    int i;
    enum { INIT, GETMNAME, HUNTING, DONE };

    switch( autos->state) {
	case INIT:
	    qprintf( ctx, "(kobold)\n");
	    qprintf( ctx, "Hunt what: ");

	case GETMNAME:
	    if (qgets( mname, sizeof(mname), ctx)) return 1; 

	    nmon = dr1Dice_roll("d6");
	    for (i=0; i<nmon; i++) {
		if ( dr1Monster_init( &m[i], mname)) {
		    qprintf( ctx, "There are no %s about.\n", mname);
		    dr1Context_popcall( ctx, -1);
		    return 0;
		}
		m[i].location.x = dr1Dice_roll( "d12-6*10");
		m[i].location.y = dr1Dice_roll( "d12-6*10");
	    }
	    {
		struct combatargs_t *args = dr1Context_pushcallv( 
		    ctx, dr1Combatv_showPage, "combatargs_t", sizeof(*args));
		args->nmon = nmon;
		args->m = m;
	    }
	    autos->state = DONE;
	    return 0;

	case DONE:
	    if ( HITPOINTS(p) <= 0 ) {
		if ( HITPOINTS(p) < -10) {
		    qprintf( ctx, "Ewww.  What a mess.\n");
		    dr1Context_popcall( ctx, 'd');
		    return 0;
		}
		dead = dr1Dice_roll( "d100");
		if (dead < 30) {
		    qprintf( ctx, "You awaken hours later, and stumble weakened back to town.\n");
		} else {
		    qprintf( ctx, "A kindly cleric found your body.  He thanks you for your donation.\n");
		    bzero( &ctx->player.purse, sizeof(ctx->player.purse));
		}
	    }
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */
    return -1;
}
