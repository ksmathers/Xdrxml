#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "playerv.h"
#include "attr.h"
#include "race.h"
#include "registry.h"
#include "dice.h"
#include "class.h"


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

static void fixestr( dr1Player *p, int reset) {
    static int estr = 0;
    /*
     * Fix enhanced strength after changing character class
     */
    if (reset) estr = 0;
    if (estr == 0) estr = dr1Dice_roll("d100");
    if (p->class == DR1C_FIGHTER && p->base_attr._str == 18) {
	p->base_attr.estr = estr;
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

/*
 * Player Generation Commands
 */
int dr1Playerv_class( dr1Player *p, int c, char **v) {
    dr1ClassType *cl;
    dr1Player pnew;
    int ccode;
    static dr1Money m[4];
    static int hits[4] = { 0, 0, 0, 0 };
    int idx;

    if (c == -1) {
	for (idx=0; idx<4; idx++) hits[idx] = 0;
	p->class = DR1C_INVALID;
	p->hp = 0;
	p->purse.gp = 0;
	return 0;
    }

    if (c != 2) return -1;
    if (!strcasecmp( v[1], "mu")) {
	ccode = DR1C_MU;
	idx = 0;
    } else if (!strcasecmp( v[1], "fighter")) {
	ccode = DR1C_FIGHTER;
	idx = 1;
    } else if (!strcasecmp( v[1], "cleric")) {
	ccode = DR1C_CLERIC;
	idx = 2;
    } else if (!strcasecmp( v[1], "thief")) {
	ccode = DR1C_THIEF;
	idx = 3;
    } else return -1;
    
    cl = dr1Registry_lookup( &dr1class, ccode);
    if (!cl) return -1;
    pnew = *p;
    pnew.class = ccode;

    if ( check_classattr( &pnew)) return -2;
    *p = pnew;

    if ( !hits[ idx]) {
	p->hp = dr1Dice_roll( cl->hitdice);
	p->purse.gp = dr1Dice_roll( cl->startingMoney);
	m[ idx] = p->purse;
	hits[ idx] = p->hp;
    } else {
        p->hp = hits[ idx];
	p->purse = m[ idx];
    }
    fixestr(p,0);
    return 0;
}

int dr1Playerv_roll( dr1Player *p, int c, char **v) {
    if (c != 1) return -1;
    p->base_attr = dr1Attr_create_mode4();
    p->curr_attr = p->base_attr;
    dr1Playerv_class( p, -1, NULL);  /* reset hitpoints & wealth */
    fixestr(p,1);
    
    apply_racesex( p);

    return 0;
}

int dr1Playerv_name( dr1Player *p, int c, char **v) {
    if (c != 2) return -1;
    if (p->name) free( p->name);
    p->name = strdup( v[1]);
    return 0;
}

int dr1Playerv_swap( dr1Player *p, int c, char **v) {
    int *a, *b, t;

    if (c != 3) return -1;
    undo_racesex(p);
    a = dr1Attr_statptr( &p->base_attr, v[1]);
    b = dr1Attr_statptr( &p->base_attr, v[2]);
    if (a == NULL || b == NULL) return -1;
    t = *a;
    *a = *b;
    *b = t;
    apply_racesex(p);
    fixestr(p,0);
    if ( check_classattr( p)<0) p->class = DR1C_INVALID;
    return 0;
}

int dr1Playerv_sex( dr1Player *p, int c, char **v) {
    dr1AttrAdjust *old, *new;
    int r;
    char s[80];
    
    if (c == 1) {
	printf("(male, female)\n");
	gets(s);
    
        v[1] = s;
	c = 2;
    }
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

int dr1Playerv_race( dr1Player *p, int c, char **v) {
    dr1AttrAdjust *old, *new;
    int r;
    char s[80];
    
    if (c == 1) {
	printf("(human, elf, halfling, dwarf, halforc)\n");
	printf("Race: ");
	gets(s);
    
        v[1] = s;
	c = 2;
    }
    if (c != 2) return -1;

    old = dr1Registry_lookup( &dr1race, p->race);

    if (!strcasecmp( v[1], "human")) r=DR1R_HUMAN;
    else if (!strcasecmp( v[1], "elf")) r=DR1R_ELF;
    else if (!strcasecmp( v[1], "halfling")) r=DR1R_HOBBIT;
    else if (!strcasecmp( v[1], "dwarf")) r=DR1R_DWARF;
    else if (!strcasecmp( v[1], "halforc")) r=DR1R_HALFORC;
    else return -1;

    new = dr1Registry_lookup( &dr1race, r);
    if (!new) return -1;
    if (old) dr1Attr_adjust( &p->base_attr, old, -1);
    dr1Attr_adjust( &p->base_attr, new, 1);
    p->race = r;
    if (check_classattr( p)<0) p->class=DR1C_INVALID;
    return 0;
}


int dr1Playerv_trade( dr1Player *p, int c, char **v) {
    int *a, *b;
    char v1[80], v2[80];

    if (c == 1) {
	printf("(str, int, wis, dex, con, cha)\n");
        printf("Trade from: ");
	gets( v1);
	printf("Trade to: ");
	gets( v2);
	v[1] = v1;
	v[2] = v2;
	c=3;
    }
    if (c != 3) return -1;
    undo_racesex(p);
    a = dr1Attr_statptr( &p->base_attr, v[1]);
    b = dr1Attr_statptr( &p->base_attr, v[2]);
    if (a == NULL || b == NULL) return -1;
    if (*a < 5 || *b > 17) return -1;
    *a -= 2;
    *b += 1;
    apply_racesex(p);
    fixestr(p,0);
    if ( check_classattr( p)<0) return -1;
    return 0;
}

int dr1Playerv_init( dr1Player *p) {
    p->name = strdup( "Unnamed");
    p->race = DR1R_HUMAN; 
    p->sex = DR1R_MALE;
    p->level = 1;
    return 0;
}

int dr1Playerv_showDialog( dr1Player *p) {
    char cmd[80];
    char *cmds[10];
    char buf[80];
    dr1AttrAdjust *race;
    dr1ClassType *class;
    int i;
    int res;

    dr1Playerv_init( p);

    for (;;) {

	i = 0;

	/*
	 * Fix possible problems after changing stats
	 */
	fixestr(p,0);

	dr1Money_format( &p->purse, buf);
	race = dr1Registry_lookup( &dr1race, p->race);
	class = dr1Registry_lookup( &dr1class, p->class);
	printf("---------------------------------------------------------\n");
	if (p->name) printf("Name: %s\n", p->name);
	if (race) printf("Race: %s\n", race->type);
	if (class) printf("Class: %s\n", class->class);
	printf("Sex: %s\n", p->sex==DR1R_FEMALE?"Female":"Male");
	printf("Purse: %s\n", buf);
	printf("Hits: %d\n", HITPOINTS(p));
	printf("Str: %2d", p->base_attr._str);
	if (p->base_attr.estr) {
	    printf("/%02d\n", p->base_attr.estr > 99 ? 0 : p->base_attr.estr);
	} else {
	    printf("\n");
	}
	printf("Int: %2d\n", p->base_attr._int);
	printf("Wis: %2d\n", p->base_attr._wis);
	printf("Dex: %2d\n", p->base_attr._dex);
	printf("Con: %2d\n", p->base_attr._con);
	printf("Cha: %2d\n\n", p->base_attr._cha);

	printf("(roll, swap, trade, race, sex, class, name, done)\n");
	printf("Command: ");
	fgets( cmd, sizeof(cmd), stdin);

	cmds[0] = strtok( cmd, " \t\n");
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );
	
	if ( !strcmp(cmds[0], "roll")) res=dr1Playerv_roll( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "swap")) res=dr1Playerv_swap( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "trade")) res=dr1Playerv_trade( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "race")) res=dr1Playerv_race( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "name")) res=dr1Playerv_name( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "class")) res=dr1Playerv_class( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "sex")) res=dr1Playerv_sex( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "done")) {
	    if ( !strcmp(p->name, "Unnamed")) {
		printf("Your character needs a name.\n");
	    } else if ( p->class == DR1C_INVALID) {
		printf("Your character has no class.\n");
	    } else if ( p->base_attr._str < 3) {
		printf("Your character has no stats.  Roll stats first.\n");
	    } else {
		break;
	    }
	} else printf("Unknown command: '%s'\n", cmds[0]);
	if (res) printf("Error %d in last command.\n", res);
    }
    return 0;
}

