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
#include "qio.h"


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

static void fixestr( dr1Player *p, int *estr) {
    /*
     * Fix enhanced strength after changing character class
     */
    if (*estr == 0) *estr = dr1Dice_roll("d100");
    if (p->class == DR1C_FIGHTER && p->base_attr._str == 18) {
	p->base_attr.estr = *estr;
    } else {
	p->base_attr.estr = 0;
    }
    p->curr_attr = p->base_attr;
}

static int check_classattr( dr1Player *p) {
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

enum { START_MU, START_THIEF, START_CLERIC, START_FIGHTER, START_MAX };

int apply_class( dr1Player *p, int ccode, int *hits, dr1Money *gold, int *estr) 
{
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

    
    if ( !hits[ idx]) {
	p->hp = dr1Dice_roll( cl->hitdice);
	p->purse.gp = dr1Dice_roll( cl->startingMoney);
	gold[ idx] = p->purse;
	hits[ idx] = p->hp;
    } else {
	p->hp = hits[ idx];
	p->purse = gold[ idx];
    }
    fixestr(p, estr);
    return 0;
}

/*
 * Player Generation Commands
 */

/* class: Choose a class for the player */
struct dr1Playerv_classargs_t {
    int c;
    char **v;
    int* hits;
    dr1Money* gold;
    int* estr;
};

int dr1Playerv_classv( dr1Context *ctx);
int dr1Playerv_class( 
	dr1Context *ctx, int c, char **v, int *hits, dr1Money *gold, int *estr
    ) 
{
    struct dr1Playerv_classargs_t *args = dr1Context_pushcallv( 
	    ctx, dr1Playerv_classv, "dr1Playerv_classargs_t", sizeof(*args)
	);
    if (!args) return -1;

    args->c = c;
    args->v = v;
    args->gold = gold;
    args->hits = hits;
    args->estr = estr;
    return 0;
}

int dr1Playerv_classv( dr1Context *ctx) {
    struct dr1Playerv_classargs_t *args = 
	    dr1Context_args( ctx, "dr1Playerv_classargs_t");
    struct {
	int state;
	char s[20];
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    dr1Player *p = &ctx->player;
    int ccode;
    int idx;
    int res;
    enum { INIT, GETCLASS, RUN };

    switch (autos->state) {

	case INIT:
	    if (args->c == 1) {
		qprintf( ctx, "(mu,fighter,cleric,thief)\n");
		qprintf( ctx, "Class: ");
		autos->state = GETCLASS;
	    } else {
		autos->state = RUN;
	    }
	    return 0;

        case GETCLASS:
	    if ( qgets( autos->s, sizeof(autos->s), ctx)) return 1;
	    args->v[1] = autos->s;
	    autos->state = RUN;

	case RUN:	
	    if (args->c != 2) {
	        dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    if (!strcasecmp( args->v[1], "mu")) {
		ccode = DR1C_MU;
		idx = 0;
	    } else if (!strcasecmp( args->v[1], "fighter")) {
		ccode = DR1C_FIGHTER;
		idx = 1;
	    } else if (!strcasecmp( args->v[1], "cleric")) {
		ccode = DR1C_CLERIC;
		idx = 2;
	    } else if (!strcasecmp( args->v[1], "thief")) {
		ccode = DR1C_THIEF;
		idx = 3;
	    } else {
		dr1Context_popcall( ctx, -1);
		return 0;
	    }
	    
	    res = apply_class( p, ccode, args->hits, args->gold, args->estr);
	    dr1Context_popcall( ctx, res);
	    return 0;
    } /* switch */
    return -1;
}

int dr1Playerv_roll( 
	dr1Context *ctx, int c, char **v, int *hits, dr1Money *gold, int *estr) 
{
    dr1Player *p = &ctx->player;
    int oldclass;
    int i;
    if (c != 1) return -1;

    oldclass = p->class;
    p->base_attr = dr1Attr_create_mode4();
    p->curr_attr = p->base_attr;

    /* reset class (reroll starting gold, etc) */
    for (i = 0; i<START_MAX; i++) {
	hits[i] = 0;
	bzero( &gold[i], sizeof(gold[i]));
	*estr = 0;
    }
    p->hp = 0;
    p->purse.gp = 0;

    apply_racesex( p);
    apply_class( p, oldclass, hits, gold, estr);
    fixestr(p,estr);


    return 0;
}

int dr1Playerv_name( dr1Context *ctx, int c, char **v) {
    dr1Player *p = &ctx->player;
    if (c != 2) return -1;
    if (p->name) free( p->name);
    p->name = strdup( v[1]);
    return 0;
}

int dr1Playerv_sex( dr1Context *ctx, int c, char **v, int *estr) {
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
int dr1Playerv_racev( dr1Context *ctx);
int dr1Playerv_race( dr1Context *ctx, int c, char **v, int *estr) {
    
    struct dr1Playerv_args_t *args = dr1Context_pushcallv( 
	    ctx, dr1Playerv_racev, "dr1Playerv_args_t", sizeof(*args)
	);
    if (!args) return -1;
    args->c = c;
    args->v = v;
    args->estr = estr;
    return 0;
}

int dr1Playerv_racev( dr1Context *ctx) {
    dr1AttrAdjust *old, *new;
    int r;
    struct dr1Playerv_args_t *args = dr1Context_args( ctx, "dr1Playerv_args_t");
    struct {
	int state;
	char s[20];
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    dr1Player *p = &ctx->player;
    enum { INIT, GETRACE, RUN };
    
    switch ( autos->state) {
	case INIT:
	    if (args->c == 1) {
		qprintf( ctx, "(human, elf, halfling, dwarf, halforc)\n");
		qprintf( ctx, "Race: ");
		autos->state = GETRACE;
		return 0;
	    }
	    autos->state = RUN;
	    return 0;

        case GETRACE:
	    if ( qgets( autos->s, sizeof(autos->s), ctx)) return 1;
	
	    args->v[1] = autos->s;
	    args->c = 2;
	    autos->state = RUN;

	case RUN:
	    if (args->c != 2) return -1;

	    old = dr1Registry_lookup( &dr1race, p->race);

	    if (!strcasecmp( args->v[1], "human")) r=DR1R_HUMAN;
	    else if (!strcasecmp( args->v[1], "elf")) r=DR1R_ELF;
	    else if (!strcasecmp( args->v[1], "halfling")) r=DR1R_HOBBIT;
	    else if (!strcasecmp( args->v[1], "dwarf")) r=DR1R_DWARF;
	    else if (!strcasecmp( args->v[1], "halforc")) r=DR1R_HALFORC;
	    else {
	        dr1Context_error( ctx, "Invalid race '%s'", args->v[1]);
	        dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    new = dr1Registry_lookup( &dr1race, r);
	    if (!new) { dr1Context_popcall( ctx, -2); return 0; }
	    if (old) dr1Attr_adjust( &p->base_attr, old, -1);
	    dr1Attr_adjust( &p->base_attr, new, 1);
	    p->race = r;

            /* apply exceptional strength if appropriate */
	    fixestr(p,args->estr);

	    if (check_classattr( p)<0) p->class=DR1C_INVALID;
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */
    dr1Context_error( ctx, "Internal error");
    return 1;
}


/* trade: Trade points in one stat for points in another */
int dr1Playerv_swapv( dr1Context *ctx);
int dr1Playerv_swap( dr1Context *ctx, int c, char **v, int *estr) {
    
    struct dr1Playerv_args_t *args = dr1Context_pushcallv( 
	    ctx, dr1Playerv_swapv, "dr1Playerv_args_t", sizeof(*args)
	);
    if (!args) return -1;
    args->c = c;
    args->v = v;
    args->estr = estr;
    return 0;
}

int dr1Playerv_swapv( dr1Context *ctx) {
    int *a, *b, t;
    dr1Player *p = &ctx->player;
    struct dr1Playerv_args_t *args = dr1Context_args( ctx, "dr1Playerv_args_t");
    struct {
	int state;
	char v1[10];
	char v2[10];
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    enum { INIT, READFROM, READTO, CONT };

    switch (autos->state) {
	case INIT:
	    if (args->c == 1) {
		qprintf( ctx, "(str, int, wis, dex, con, cha)\n");
		qprintf( ctx, "Swap: ");
		autos->state = READFROM;
		return 0;
	    }
            autos->state = CONT;
	    return 0;

	case READFROM:
	    if (qgets( autos->v1, sizeof(autos->v1), ctx)) return 1;
	    qprintf( ctx, "With: ");
	    autos->state = READTO;
	    return 0;

	case READTO:
	    if (qgets( autos->v2, sizeof(autos->v2), ctx)) return 1;
	    args->v[1] = autos->v1;
	    args->v[2] = autos->v2;
	    args->c = 3;
	    autos->state = CONT;

        case CONT:
	    if (args->c != 3) return -1;

	    /* get attribute pointers */
	    a = dr1Attr_statptr( &p->base_attr, args->v[1]);
	    b = dr1Attr_statptr( &p->base_attr, args->v[2]);
	    if (a == NULL || b == NULL) {
		dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    /* swap attributes */
	    undo_racesex(p);
	    t = *a;
	    *a = *b;
	    *b = t;
	    apply_racesex(p);

            /* apply exceptional strength if appropriate */
	    fixestr(p,args->estr);

            /* reset class if attributes are out of range */
	    if ( check_classattr( p)<0) p->class = DR1C_INVALID;
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */
    return -1;
}

/* trade: Trade points in one stat for points in another */
int dr1Playerv_tradev( dr1Context *ctx);
int dr1Playerv_trade( dr1Context *ctx, int c, char **v, int *estr) {
    
    struct dr1Playerv_args_t *args = dr1Context_pushcallv( 
	    ctx, dr1Playerv_tradev, "dr1Playerv_args_t", sizeof(*args)
	);
    if (!args) return -1;
    args->c = c;
    args->v = v;
    args->estr = estr;
    return 0;
}

int dr1Playerv_tradev( dr1Context *ctx) {
    int *a, *b;
    struct dr1Playerv_args_t *args = dr1Context_args( ctx, "dr1Playerv_args_t");
    struct {
	int state;
	char v1[10];
	char v2[10];
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    enum { INIT, READFROM, READTO, CONT };

    switch (autos->state) {
	case INIT:
	    if (args->c == 1) {
		qprintf( ctx, "(str, int, wis, dex, con, cha)\n");
		qprintf( ctx, "Trade from: ");
		autos->state = READFROM;
		return 0;
	    }
            autos->state = CONT;
	    return 0;

	case READFROM:
	    if (qgets( autos->v1, sizeof(autos->v1), ctx)) return 1;
	    qprintf( ctx, "Trade to: ");
	    autos->state = READTO;
	    return 0;

	case READTO:
	    if (qgets( autos->v2, sizeof(autos->v2), ctx)) return 1;
	    args->v[1] = autos->v1;
	    args->v[2] = autos->v2;
	    args->c = 3;
	    autos->state = CONT;

        case CONT:
	    if (args->c != 3) {
		dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    /* get stat pointers */
	    a = dr1Attr_statptr( &ctx->player.base_attr, args->v[1]);
	    b = dr1Attr_statptr( &ctx->player.base_attr, args->v[2]);
	    if (a == NULL || b == NULL) { 
		/* unknown stat */
		dr1Context_popcall( ctx, -2); 
		return 0; 
	    }

	    if (*a < 5 || *b > 17) { 
		/* can't trade for more than 18, or less than 3 */
		dr1Context_popcall( ctx, -3); return 0; 
	    }

	    /* modify the stats */
	    undo_racesex( &ctx->player);
	    *a -= 2;
	    *b += 1;
	    apply_racesex( &ctx->player);

            /* calculate exceptional strength if appropriate */
	    fixestr( &ctx->player, args->estr);

	    /* check if the stats are in bounds for your class */
	    if ( check_classattr( &ctx->player)<0) { 
		dr1Context_popcall( ctx, -4); return 0; 
	    }

	    /* return to main */
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */
    errno = ENOSYS;
    return 1;
}

int dr1Playerv_init( dr1Player *p) {
    p->name = strdup( "Unnamed");
    p->race = DR1R_HUMAN; 
    p->sex = DR1R_MALE;
    p->level = 1;
    return 0;
}

int dr1Playerv_showDialog( dr1Context *ctx) {
    char buf[80];
    dr1AttrAdjust *race;
    dr1ClassType *class;
    int i;
    int *res = &ctx->cstack[ ctx->stackptr-1].result;
    dr1Player *p = &ctx->player;
    struct {
        char cmd[80];
	char *cmds[10];
	int nargs;
	int state;
	int hits[START_MAX];
	dr1Money gold[START_MAX];
	int estr;
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    enum { INIT, SHOWPAGE, CMDPROMPT, GETCMD, SHOWRES };

    printf("%d: dr1Playerv_showDialog\n", fileno( ctx->fp));

    switch (autos->state) {
        case INIT:
	    dr1Playerv_init( p);
	    autos->state = SHOWPAGE;
	    return 0;

	case SHOWPAGE:
	    /*
	     * Fix possible problems after changing stats
	     */
	    fixestr(p, &autos->estr);

	    dr1Money_format( &p->purse, buf);
	    race = dr1Registry_lookup( &dr1race, p->race);
	    class = dr1Registry_lookup( &dr1class, p->class);
	    qprintf( ctx, "---------------------------------------------------------\n");
	    if (p->name) qprintf( ctx, "Name: %s\n", p->name);
	    if (race) qprintf( ctx, "Race: %s\n", race->type);
	    if (class) qprintf( ctx, "Class: %s\n", class->class);
	    qprintf( ctx, "Sex: %s\n", p->sex==DR1R_FEMALE?"Female":"Male");
	    qprintf( ctx, "Purse: %s\n", buf);
	    qprintf( ctx, "Hits: %d\n", HITPOINTS(p));
	    qprintf( ctx, "Str: %2d", p->base_attr._str);
	    if (p->base_attr.estr) {
		qprintf( ctx, "/%02d\n", p->base_attr.estr > 99 ? 0 : p->base_attr.estr);
	    } else {
		qprintf( ctx, "\n");
	    }
	    qprintf( ctx, "Int: %2d\n", p->base_attr._int);
	    qprintf( ctx, "Wis: %2d\n", p->base_attr._wis);
	    qprintf( ctx, "Dex: %2d\n", p->base_attr._dex);
	    qprintf( ctx, "Con: %2d\n", p->base_attr._con);
	    qprintf( ctx, "Cha: %2d\n\n", p->base_attr._cha);

	    qprintf( ctx, "(roll, swap, trade, race, sex, class, name, done)\n");
	case CMDPROMPT:
	    qprintf( ctx, "Command: ");
	    autos->state = GETCMD;
	    return 0;

	case GETCMD:
	    if (qgets( autos->cmd, sizeof(autos->cmd), ctx)) return 1;

            /* tokenize the command */
	    autos->cmds[0] = strtok( autos->cmd, " \t\n");
	    i=0;
	    while ((autos->cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );
            autos->nargs = i;

	    /* interpret command */
	    *res = 0;
	    if ( !strcmp(autos->cmds[0], "roll")) {
		*res=dr1Playerv_roll( ctx, autos->nargs, autos->cmds, 
			autos->hits, autos->gold, &autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "swap")) {
		*res=dr1Playerv_swap( ctx, autos->nargs, autos->cmds, 
			&autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "trade")) {
		*res=dr1Playerv_trade( ctx, autos->nargs, autos->cmds, 
			&autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "race")) {
		*res=dr1Playerv_race( ctx, autos->nargs, autos->cmds, 
			&autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "name")) {
		*res=dr1Playerv_name( ctx, autos->nargs, autos->cmds);
	    } else if ( !strcasecmp(autos->cmds[0], "class")) { 
		*res=dr1Playerv_class( ctx, autos->nargs, autos->cmds, 
			autos->hits, autos->gold, &autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "sex")) {
		*res=dr1Playerv_sex( ctx, autos->nargs, autos->cmds, 
			&autos->estr);
	    } else if ( !strcasecmp(autos->cmds[0], "done")) {
		if ( !strcmp(p->name, "Unnamed")) {
		    qprintf( ctx, "Your character needs a name.\n");
		} else if ( p->class == DR1C_INVALID) {
		    qprintf( ctx, "Your character has no class.\n");
		} else if ( p->base_attr._str < 3) {
		    qprintf( ctx, "Your character has no stats.  Roll stats first.\n");
		} else {
		    dr1Context_popcall( ctx, 0);
		    return 0;
		}
	    } else qprintf( ctx, "Unknown command: '%s'\n", autos->cmds[0]);
	    autos->state = SHOWRES;
	    return 0;

	case SHOWRES:	
	    if (*res) {
	        autos->state = CMDPROMPT;
		qprintf( ctx, "Error %d in last command.\n", *res);
	    } else {
		autos->state = SHOWPAGE;
	    }
	    return 0;
    }
    return 1;
}

