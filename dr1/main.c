#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "player.h"
#include "playerv.h"
#include "apothecary.h"
#include "smithy.h"
#include "tanner.h"
#include "wright.h"
#include "money.h"
#include "barter.h"
#include "dice.h"
#include "xdrasc.h"
#include "itemset.h"
#include "monster.h"
#include "combatv.h"

int buy( dr1Player *p, int c, char **v) {
    /* purchase an item */
    char *what;
    char *who;
    dr1Merchant *merc;
    dr1Money val;
    dr1Barter* b;
    char* offr;
    long sell, buy;
    char mbuf[ 40];
    char buf[ 100];
    char _who[80];
    char _what[80];
    int len;
    int i;

    if (c == 1) {
	printf("(fnord, gandor, frommer, wright)\n");
	printf("From whom:");
	gets( _who);
	v[1]= _who;
	c = 2;
    }

    who = v[1];
    if ( !strcasecmp( who, "Fnord")) merc = &dr1apothecary;
    else if ( !strcasecmp( who, "Gandor")) merc = &dr1smithy;
    else if ( !strcasecmp( who, "Frommer")) merc = &dr1tanner;
    else if ( !strcasecmp( who, "Wright")) merc = &dr1wright;
    else return -1;

    
    printf("Welcome to '%s'\n", merc->store);
    if (merc->mood > 10) {
	printf("%s looks a bit peeved today.\n", merc->name);
    }
    if (c == 2) {
	putchar('(');
        for (i=0; i<merc->itemStore.len; i++) {
	    if (i) putchar(',');
	    printf("%s", merc->itemStore.items[i]->name);
	}
	printf(")\n");
	printf("Buy what:");
	gets( _what);
	v[2]= _what;
	c = 3;
    }

    if (c != 3) return -1;
    what = v[2];

    b = dr1Merchant_buy( merc, what);
    if (!b) return -1;
    offr = dr1Barter_startingOffer( b);
    printf("You have %dgp\n", p->purse.gp);
    printf("Buying %s\n", what);
    do {
/*        printf("mood %d\n", dr1apothecary.mood); /**/
        dr1Money_format( &b->start, mbuf);
	printf( "%s %s\n", offr, mbuf);
	printf("You offer? ");
	fgets( buf, sizeof(buf), stdin);
	len = strlen(buf);
	dr1Money_parse( &val, buf);
	buy = dr1Money_value( &val, b->merchant->ep);
	dr1Money_format( &val, mbuf);
	printf("money %s\n", mbuf);
	offr = dr1Barter_offer( b, val);
	sell = dr1Money_value( &b->start, b->merchant->ep);
    } while ( buy < sell && buy != 0);
    if ( buy==0) {
	printf("%s\n", (char *) offr);
    } else if ( dr1Money_deduct( &p->purse, &b->start)) {
	printf("Sorry, you haven't that much in your purse.\n");
    } else {
	printf("%s\n", (char *) offr);
	dr1Merchant_completeSale( &dr1apothecary, b);
	dr1ItemSet_add( &p->pack, b->item);
    }
    free(b);
    return 0;
}

int equip( dr1Player *p, int c, char **v) {
    /* equip armor, weapons, etc. */
    dr1Item *item;
    dr1Item *unequip = NULL;

    if (c != 2) return -1;
    item = dr1ItemSet_findName( &p->pack, v[1]);
    if (!item) return -1;
    
    if (item->weapon) {
	if (p->weapon) {
	    unequip = &p->weapon->super;
	    p->weapon = NULL;
	}
	p->weapon = (dr1Weapon*)item;
    } else if ( dr1Item_isArmor( item)) {
        if (p->armor) {
	    unequip = &p->armor->super;
	    p->armor = NULL;
	}   
	p->armor = (dr1Armor*)item;
    } else return -2;

    dr1ItemSet_remove( &p->pack, item);
          
    if (unequip) {
	printf("Putting %s back in your pack.\n", unequip->name);
	dr1ItemSet_add( &p->pack, unequip);
    }
    printf("Equipping %s.\n", item->name);
    
    return 0;
}

int hunt( dr1Player *p, int c, char **v) {
    char buf[80];
    char *mname;
    dr1Monster m[10];
    int nmon;
    int i;

    if (c < 2) {
	printf("Hunt what: ");
        gets( buf);
	v[1] = buf;
	c = 2;
    }

    if (c != 2) return -1;
    mname = v[1];

    nmon = dr1Dice_roll( "d4");
    for (i=0; i<nmon; i++) {
	if ( dr1Monster_init( &m[i], mname)) return -2;
    }
    
    dr1Combatv_showPage( p, nmon, m);
    return 0;
}

int rest( dr1Player *p, int c, char **v) {
    dr1Money roomcost = { 0, 2, 0, 0, 0 };
    printf("You've rested for 1 day at 2sp per day for room and board.\n");
    p->wounds --;
    if (p->wounds < 0) p->wounds = 0;
    dr1Money_deduct( &p->purse, &roomcost);
    return 0;
}

int main( int argc, char** argv) {
    FILE *fp;
    XDR xdrs;
    dr1Player player;
    struct stat pdat;
    int pdatf;
    dr1Player *loadOk;
    
    /* initialize globals */
    bzero( &player, sizeof(player));
    dr1Dice_seed();
    dr1Apothecary_init( &dr1apothecary);
    dr1Tanner_init();
    dr1Wright_init();
    dr1Smithy_init();

    /* restore player from disk */
    pdatf = stat( "player.dat", &pdat);

    if (!pdatf && S_ISREG(pdat.st_mode)) {
	loadOk = dr1Player_load( &player, "player.dat");
	assert(loadOk);
    } else {
        dr1Playerv_showDialog( &player);
    }

    
    for (;;) {
        char cmd[100];
	char *cmds[10];
	int i, r;
	dr1Player *p = &player;
	i=0;

	printf("---------------------------------------------------------\n");
	if (player.name) printf("Name: %s\n", player.name);

	printf("(buy, equip, sell, rest, hunt, quit)\n");
	printf("Command: ");
	fgets( cmd, sizeof(cmd), stdin);

	cmds[0] = strtok( cmd, " \t\n");
	if (!cmds[0]) continue;
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );
	
	if ( !strcmp(cmds[0], "buy")) r=buy( p, i, cmds);
	else if ( !strcmp(cmds[0], "equip")) r=equip( p, i, cmds);
	else if ( !strcmp(cmds[0], "hunt")) r=hunt( p, i, cmds);
	else if ( !strcmp(cmds[0], "rest")) r=rest( p, i, cmds);
	else if ( !strcmp(cmds[0], "quit")) break;
	else printf("Unknown command: '%s'\n", cmds[0]);
	if (r) printf("Command returned code %d\n", r);
    }
  
    /* dump player */
    printf("Player\n");
    xdr_push_note( &xdrasc, "player");
    xdr_dr1Player( &xdrasc, &player);
    xdr_pop_note( &xdrasc);

    /* save player with new item */
    fp = fopen("player.dat", "w");
    xdrstdio_create( &xdrs, fp, XDR_ENCODE);
    xdr_dr1Player( &xdrs, &player);
    xdr_destroy( &xdrs);
    fclose( fp);
    
    return 0;
} // main
