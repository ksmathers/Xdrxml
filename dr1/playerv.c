#include <stdio.h>
#include <string.h>

#include "playerv.h"
#include "attr.h"
#include "race.h"
#include "registry.h"
#include "dice.h"
#include "class.h"

static int *statptr( dr1Attr *a, char *s) {
    if ( !strcasecmp( s, "str")) return &a->_str;
    if ( !strcasecmp( s, "int")) return &a->_int;
    if ( !strcasecmp( s, "wis")) return &a->_wis;
    if ( !strcasecmp( s, "dex")) return &a->_dex;
    if ( !strcasecmp( s, "con")) return &a->_con;
    if ( !strcasecmp( s, "cha")) return &a->_cha;
    printf("Unknown attribute '%s'\n", s);
    return NULL;
}

int dr1Playerv_roll( dr1Player *p, int c, char **v) {
    if (c != 1) return -1;
    p->base_attr = dr1Attr_create_mode4();
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
    a = statptr( &p->base_attr, v[1]);
    b = statptr( &p->base_attr, v[2]);
    if (a == NULL || b == NULL) return -1;
    t = *a;
    *a = *b;
    *b = t;
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

    if (!strcasecmp( v[1], "male")) r='MALE';
    else if (!strcasecmp( v[1], "female")) r='FEMA';
    else return -1;

    new = dr1Registry_lookup( &dr1race, r);
    if (!new) return -1;
    if (old) dr1Attr_adjust( &p->base_attr, &old->offset, -1);
    dr1Attr_adjust( &p->base_attr, &new->offset, 1);
    p->race = r;
    return 0;
}

int dr1Playerv_race( dr1Player *p, int c, char **v) {
    dr1AttrAdjust *old, *new;
    int r;
    char s[80];
    
    if (c == 1) {
	printf("(human, elf, halfling)\n");
	gets(s);
    
        v[1] = s;
	c = 2;
    }
    if (c != 2) return -1;

    old = dr1Registry_lookup( &dr1race, p->race);

    if (!strcasecmp( v[1], "human")) r='MAN ';
    else if (!strcasecmp( v[1], "elf")) r='ELF ';
    else if (!strcasecmp( v[1], "halfling")) r='HOBB';
    else return -1;

    new = dr1Registry_lookup( &dr1race, r);
    if (!new) return -1;
    if (old) dr1Attr_adjust( &p->base_attr, &old->offset, -1);
    dr1Attr_adjust( &p->base_attr, &new->offset, 1);
    p->race = r;
    return 0;
}

int dr1Playerv_class( dr1Player *p, int c, char **v) {
    dr1ClassType *cl;
    int ccode;

    if (c != 2) return -1;
    if (!strcasecmp( v[1], "mu")) {
	ccode = 'MU  ';
    } else if (!strcasecmp( v[1], "fighter")) {
	ccode = 'FIGH';
    } else if (!strcasecmp( v[1], "cleric")) {
	ccode = 'CLER';
    } else if (!strcasecmp( v[1], "thief")) {
	ccode = 'THIE';
    } else return -1;
    
    cl = dr1Registry_lookup( &dr1class, ccode);
    if (!cl) return -1;

    p->class = ccode;
    p->hp = dr1Dice_roll( cl->hitdice);
    p->purse.gp = dr1Dice_roll( cl->startingMoney);
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
    a = statptr( &p->base_attr, v[1]);
    b = statptr( &p->base_attr, v[2]);
    if (a == NULL || b == NULL) return -1;
    if (*a < 5 || *b > 17) return -1;
    *a -= 2;
    *b += 1;
    return 0;
}

int dr1Playerv_init( dr1Player *p) {
    p->name = strdup( "Unnamed");
    p->race = 'MAN ';
    p->sex = 'MALE';
    return 0;
}

int dr1Playerv_showDialog( dr1Player *p) {
    char cmd[80];
    char *cmds[10];
    char buf[80];
    dr1AttrAdjust *race;
    dr1ClassType *class;
    int i;

    dr1Playerv_init( p);

    for (;;) {

	i = 0;

	dr1Money_format( &p->purse, buf);
	race = dr1Registry_lookup( &dr1race, p->race);
	class = dr1Registry_lookup( &dr1class, p->class);
	printf("---------------------------------------------------------\n");
	if (p->name) printf("Name: %s\n", p->name);
	if (race) printf("Race: %s\n", race->type);
	if (class) printf("Class: %s\n", class->class);
	printf("Sex: %s\n", p->sex=='FEMA'?"Female":"Male");
	printf("Purse: %s\n", buf);
	printf("Str: %2d\n", p->base_attr._str);
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
	
	if ( !strcmp(cmds[0], "roll")) dr1Playerv_roll( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "swap")) dr1Playerv_swap( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "trade")) dr1Playerv_trade( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "race")) dr1Playerv_race( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "name")) dr1Playerv_name( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "class")) dr1Playerv_class( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "sex")) dr1Playerv_sex( p, i, cmds);
	else if ( !strcasecmp(cmds[0], "done")) return 0;
	else printf("Unknown command: '%s'\n", cmds[0]);
    }
    printf("class=%08x\n", p->class);
}
