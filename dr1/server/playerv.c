#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "playerv.h"
#include "attr.h"
#include "race.h"
#include "registry.h"
#include "dice.h"
#include "class.h"
#include "context.h"
#include "dr1.h"


/* struct representing arguments to the playerv command calls */
struct dr1Playerv_args_t {
    int c; char **v;
    int *estr;
};

/*
 * Utility functions
 */

static void apply_racesex( dr1Player *p) {
    /* reapply race and sex adjustments for new attributes */
    dr1AttrAdjust *adj;

    adj = dr1Registry_lookup( &dr1race, p->race);
    if (adj) dr1Attr_adjust( &p->base_attr, adj, 1);
    adj = dr1Registry_lookup( &dr1sex, p->sex);
    if (adj) dr1Attr_adjust( &p->base_attr, adj, 1);
}

static void undo_racesex( dr1Player *p) {
    /* undo race and sex adjustments for new attributes */
    dr1AttrAdjust *adj;

    adj = dr1Registry_lookup( &dr1race, p->race);
    if (adj) dr1Attr_adjust( &p->base_attr, adj, -1);
    adj = dr1Registry_lookup( &dr1sex, p->sex);
    if (adj) dr1Attr_adjust( &p->base_attr, adj, -1);
}

static void fixestr( dr1Player *p, struct dr1StartingValues *sv) {
    /*
     * Fix enhanced strength after changing character class
     */
    if (sv->estr == 0) sv->estr = dr1Dice_roll("d100");
    if (p->class == DR1C_FIGHTER && p->base_attr._str == 18) {
	p->base_attr.estr = sv->estr;
    } else {
	p->base_attr.estr = 0;
    }
    p->curr_attr = p->base_attr;
}

static int check_classattr( dr1Player *p) {
    /*
     * Test that character class attribute requirements 
     * Return 0 if OK 
     * Return -1 if attributes aren't within class requirements
     * Return 1 if no class has been selected yet.
     */


    int *stat, *min;
    enum Attribute i;
    dr1ClassType *cl;

    cl = dr1Registry_lookup( &dr1class, p->class);
    if (!cl) {
	/* no class selected yet */
	return 1;
    }
    for (i=STRENGTH; i<=CHARISMA; i++) {
	stat = dr1Attr_estatptr( &p->base_attr, i);
	min = dr1Attr_estatptr( &cl->minimum, i);
	assert(stat);
	assert(min);
        if (*stat < *min) {
	    printf("Your %s is too low to be a %s\n", dr1attribute[i], cl->class);
	    return -1;
	}
    }
    return 0;
}

static int apply_class( dr1Player *p, int ccode, struct dr1StartingValues *sv) 
{
    /*
     * Apply the starting values for a particular character class
     * Class specific starting values include: gold, hitpoints, and
     * exceptional strength.
     *
     * These starting values are stored in external arrays so that
     * a player cannot select another class and then come back to this
     * class to reroll starting values (which would be cheating).
     */
    int idx;
    dr1Player pnew;
    dr1ClassType *cl;

    pnew = *p;
    pnew.class = ccode;

    switch (ccode) {
	case DR1C_MU:
	case DR1C_ILLUSIONIST:
	    idx = START_MU;
	    break;
	case DR1C_THIEF:
	case DR1C_ASASSIN:
	    idx = START_THIEF;
	    break;
	case DR1C_CLERIC:
	case DR1C_DRUID:
	case DR1C_MONK:
	    idx = START_CLERIC;
	    break;
	case DR1C_FIGHTER:
	case DR1C_PALADIN:
	case DR1C_RANGER:
	    idx = START_FIGHTER;
	    break;
	default:
	    return -1;	    
    }

    cl = dr1Registry_lookup( &dr1class, ccode);
    if (!cl) {
	return -1;
    }

    if ( check_classattr( &pnew)) {
	return -1;
    }
    *p = pnew;

    
    if ( !sv->hits[ idx]) {
	p->hp = dr1Dice_roll( cl->hitdice);
	p->purse.gp = dr1Dice_roll( cl->startingMoney);
	sv->gold[ idx] = p->purse;
	sv->hits[ idx] = p->hp;
    } else {
	p->hp = sv->hits[ idx];
	p->purse = sv->gold[ idx];
    }
    fixestr(p, sv);
    return 0;
}

/*
 * Player Generation Commands
 */

/* class: Choose a class for the player */
int dr1Playerv_class( dr1Context *ctx, int c, char **v) 
{
    dr1Player *p = &ctx->player;
    int ccode;
    int res;

    if (c != 2) {
	psendMessage( &ctx->ios, DR1MSG_190, "(mu,fighter,cleric,thief)");
	return -1;
    }

    if (!strcasecmp( v[1], "mu")) {
	ccode = DR1C_MU;
    } else if (!strcasecmp( v[1], "fighter")) {
	ccode = DR1C_FIGHTER;
    } else if (!strcasecmp( v[1], "cleric")) {
	ccode = DR1C_CLERIC;
    } else if (!strcasecmp( v[1], "thief")) {
	ccode = DR1C_THIEF;
    } else {
	psendMessage( &ctx->ios, DR1MSG_590, v[1]);
	return -2;
    }
    
    res = apply_class( p, ccode, &ctx->creationstate);
    return 0;
}

int dr1Playerv_roll( dr1Context *ctx, int c, char **v)
{
    dr1Player *p = &ctx->player;
    if (c > 1) return -1;

    /* reroll attributes */
    p->base_attr = dr1Attr_create_mode4();
    p->curr_attr = p->base_attr;

    /* reset class variables */
    dr1Context_resetCreationState( ctx);
    p->hp = 0;
    p->purse.gp = 0;

    /* reapply original sex, race, and class adjustments */
    apply_racesex( p);
    apply_class( p, p->class, &ctx->creationstate);
    return 0;
}

int dr1Playerv_name( dr1Context *ctx, int c, char **v) {
    dr1Player *p = &ctx->player;
    if (c != 2) return -1;
    if (p->name) free( p->name);
    p->name = strdup( v[1]);
    return 0;
}

int dr1Playerv_sex( dr1Context *ctx, int c, char **v) {
    dr1AttrAdjust *old, *new;
    int r;
    dr1Player *p = &ctx->player;
    
    if (c != 2) return -1;

    old = dr1Registry_lookup( &dr1sex, p->sex);

    printf("old = %p\n", old);

    if (!strcasecmp( v[1], "male")) r=DR1R_MALE;
    else if (!strcasecmp( v[1], "female")) r=DR1R_FEMALE;
    else return -1;

    new = dr1Registry_lookup( &dr1sex, r);

    printf("new = %p\n", new);

    if (!new) return -1;
    if (old) dr1Attr_adjust( &p->base_attr, old, -1);
    dr1Attr_adjust( &p->base_attr, new, 1);
    p->sex = r;
    if (check_classattr( p)<0) p->class=DR1C_INVALID;
    return 0;
}

/* race: Choose a race for the player */
int dr1Playerv_race( dr1Context *ctx, int c, char **v) {
    dr1AttrAdjust *old, *new;
    int r;
    dr1Player *p = &ctx->player;
    
    if (c != 2) {
	dr1Stream_printf( &ctx->ios, "(human, elf, halfling, dwarf, halforc)\n");
	dr1Stream_printf( &ctx->ios, "Race: ");
	return -1;
    }

    old = dr1Registry_lookup( &dr1race, p->race);

    if (!strcasecmp( v[1], "human")) r=DR1R_HUMAN;
    else if (!strcasecmp( v[1], "elf")) r=DR1R_ELF;
    else if (!strcasecmp( v[1], "halfling")) r=DR1R_HOBBIT;
    else if (!strcasecmp( v[1], "dwarf")) r=DR1R_DWARF;
    else if (!strcasecmp( v[1], "halforc")) r=DR1R_HALFORC;
    else {
	dr1Context_error( ctx, "Invalid race '%s'", v[1]);
	dr1Stream_printf( &ctx->ios, "Invalid race '%s'", v[1]);
	return -1;
    }

    new = dr1Registry_lookup( &dr1race, r);
    if (!new) { return -1; }
    if (old) dr1Attr_adjust( &p->base_attr, old, -1);
    dr1Attr_adjust( &p->base_attr, new, 1);
    p->race = r;

    /* apply exceptional strength if appropriate */
    fixestr(p,&ctx->creationstate);

    if (check_classattr( p)<0) p->class=DR1C_INVALID;
    return 0;
}


/* trade: Trade points in one stat for points in another */
int dr1Playerv_swap( dr1Context *ctx, int c, char **v) {
    int *a, *b, t;
    dr1Player *p = &ctx->player;

    if (c != 3) {
	psendMessage( &ctx->ios, DR1MSG_190, "(str, int, wis, dex, con, cha)");
	return -1;
    }

    /* get attribute pointers */
    a = dr1Attr_statptr( &p->base_attr, v[1]);
    b = dr1Attr_statptr( &p->base_attr, v[2]);
    if (a == NULL) {
	psendMessage( &ctx->ios, DR1MSG_590, v[1]);
	return -2;
    }
    if (b == NULL) {
	psendMessage( &ctx->ios, DR1MSG_590, v[2]);
	return -3;
    }

    /* swap attributes */
    undo_racesex(p);
    t = *a;
    *a = *b;
    *b = t;
    apply_racesex(p);

    /* apply exceptional strength if appropriate */
    fixestr(p,&ctx->creationstate);

    /* reset class if attributes are out of range */
    if ( check_classattr( p)<0) p->class = DR1C_INVALID;
    return 0;
}

/* improve: Improve points in one stat at the cost of points in another */
int dr1Playerv_improve( dr1Context *ctx, int c, char **v) {
    int *a, *b;

    if (c != 3) {
	psendMessage( &ctx->ios, DR1MSG_190, "(str, int, wis, dex, con, cha)");
	return -1;
    }

    /* get stat pointers */
    a = dr1Attr_statptr( &ctx->player.base_attr, v[1]);
    b = dr1Attr_statptr( &ctx->player.base_attr, v[2]);
    if (a == NULL) {
	psendMessage( &ctx->ios, DR1MSG_590, v[1]);
	return -2;
    }
    if (b == NULL) {
	psendMessage( &ctx->ios, DR1MSG_590, v[2]);
	return -3;
    }

    /* check for legal improvement */
    undo_racesex( &ctx->player);
    if (*a < 5) { 
	/* can't decrease a stat to less than 3 */
	apply_racesex( &ctx->player);
	psendMessage( &ctx->ios, DR1MSG_590, v[1]);
	return -4;
    }
    if (*b > 17) { 
	/* can't improve a stat to more than 18 */
	apply_racesex( &ctx->player);
	psendMessage( &ctx->ios, DR1MSG_590, v[2]);
	return -5;
    }

    /* modify the stats */
    *a -= 2;
    *b += 1;
    apply_racesex( &ctx->player);

    /* calculate exceptional strength if appropriate */
    fixestr( &ctx->player, &ctx->creationstate);

    /* check if the stats are in bounds for your class */
    if ( check_classattr( &ctx->player)<0) ctx->player.class = DR1C_INVALID;

    return 0;
}

int dr1Playerv_init( dr1Player *p) {
    p->name = strdup( "Unnamed");
    p->race = DR1R_HUMAN; 
    p->sex = DR1R_MALE;
    p->level = 1;
    p->location.mapname = strdup( "nomap");
    return 0;
}

int dr1Playerv_cmd( dr1Context *ctx, int c, char **v) {
    /*
     * Execute a player character creation command.  
     * Returns 0 on success
     * Returns -1 on fatal error
     */
    char buf[80];
    int res;
    dr1Player *p = &ctx->player;

    printf("%d: dr1Playerv_showDialog\n", ctx->ios.fd);


    if (c == 0) {
        psendMessage( &ctx->ios, DR1MSG_190, "(roll, swap, improve, race, sex, class, name, accept)");
	return 0;
    }

    /* interpret command */
    res = 0;
    if ( !strcasecmp(v[0], "newplayer")) {
	dr1Playerv_init( p);
	dr1Playerv_roll( ctx, 0, NULL);
	psendMessage( &ctx->ios, DR1MSG_105);
    } else if ( !strcasecmp(v[0], "roll")) {
	res=dr1Playerv_roll( ctx, c, v);
    } else if ( !strcasecmp(v[0], "swap")) {
	res=dr1Playerv_swap( ctx, c, v);
    } else if ( !strcasecmp(v[0], "improve")) {
	res=dr1Playerv_improve( ctx, c, v);
    } else if ( !strcasecmp(v[0], "race")) {
	res=dr1Playerv_race( ctx, c, v);
    } else if ( !strcasecmp(v[0], "name")) {
	res=dr1Playerv_name( ctx, c, v);
    } else if ( !strcasecmp(v[0], "class")) { 
	res=dr1Playerv_class( ctx, c, v);
    } else if ( !strcasecmp(v[0], "sex")) {
	res=dr1Playerv_sex( ctx, c, v);
    } else if ( !strcasecmp(v[0], "done")) {
	if ( !strcmp(p->name, "Unnamed")) {
	    psendMessage( &ctx->ios, DR1MSG_310, DR1ENONAME, "Your character needs a name.");
	    return 0;
	} else if ( p->class == DR1C_INVALID) {
	    psendMessage( &ctx->ios, DR1MSG_310, DR1ENOCLASS, "Your character has no class.");
	    return 0;
	} else if ( p->base_attr._str < 3) {
	    psendMessage( &ctx->ios, DR1MSG_310, DR1ENOSTATS, "Your character has no stats.  Roll stats first.\n");
	    return 0;
	} else {
	    /* ok */
	    ctx->dialog = DONE;
	    psendMessage( &ctx->ios, DR1MSG_100);
	    return 0;
	}
    } else {
	psendMessage( &ctx->ios, DR1MSG_530, v[0], "dr1Playerv_cmd");
	return 0;
    }

    if (res) {
	psendMessage( &ctx->ios, DR1MSG_560, res);
	return 0;
    } 

    /*
     * Fix possible problems after changing stats
     */
    fixestr(p, &ctx->creationstate);
    dr1Money_format( &p->purse, buf);
    sendplayer( ctx);

    return 0;
}

