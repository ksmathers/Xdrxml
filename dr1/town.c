#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "town.h"
#include "player.h"
#include "apothecary.h"
#include "smithy.h"
#include "tanner.h"
#include "wright.h"
#include "money.h"
#include "barter.h"
#include "dice.h"
#include "itemset.h"
#include "qio.h"
#include "lib/protocol.h"

int dobuy( dr1Context *ctx, int c, char **v) {
    /* purchase an item */
    dr1Player *p = &ctx->player;
    dr1Money val;
    long sell, buy;
    char mbuf[ 40];
    char buf[ 100];
    int many;
    int i;

    /* jump table */
#   define ENTRY(x) x: autos->state = x;
    enum { INIT, GETWHO, GETWHAT, GETMANY, GETOFFER };
    if (autos->state == INIT) goto INIT;
    else if (autos->state == GETWHO) goto GETWHO;
    else if (autos->state == GETWHAT) goto GETWHAT;
    else if (autos->state == GETMANY) goto GETMANY;
    else if (autos->state == GETOFFER) goto GETOFFER;
    /* end jump table */
    
ENTRY(INIT)
    if (c == 1) {
	dr1Stream_printf( &ctx->ios, "Shops in Dragon's Reach:\n");
	dr1Stream_printf( &ctx->ios, " - Fnord the Apothecary\n");
	dr1Stream_printf( &ctx->ios, " - Gandor the Blacksmith\n");
	dr1Stream_printf( &ctx->ios, " - John Frommer's Leathers\n");
	dr1Stream_printf( &ctx->ios, " - John the Wheel Wright\n\n");
	dr1Stream_printf( &ctx->ios, "(fnord, gandor, frommer, wright)\n");
	dr1Stream_printf( &ctx->ios, "Buy from whom:");

ENTRY(GETWHO)
	if (qgets( autos->_who, sizeof(autos->_who), ctx)) return 1;
	v[1]= autos->_who;
	c = 2;
    } 

    if ( !strcasecmp( v[1], "Fnord")) {
	autos->merc = &dr1apothecary;
    } else if ( !strcasecmp( v[1], "Gandor")) {
	autos->merc = &dr1smithy;
    } else if ( !strcasecmp( v[1], "Frommer")) {
	autos->merc = &dr1tanner;
    } else if ( !strcasecmp( v[1], "Wright")) {
	autos->merc = &dr1wright;
    } else {
	dr1Context_popcall( ctx, -1);
	return 0;
    }

    dr1Stream_printf( &ctx->ios, "Welcome to '%s'\n", autos->merc->store);
    if (autos->merc->mood > 10) {
	dr1Stream_printf( &ctx->ios, "%s looks a bit peeved today.\n", 
		autos->merc->name);
    }
    if (c == 2) {
	dr1Stream_printf( &ctx->ios, "(");
	for (i=0; i<autos->merc->itemStore.len; i++) {
	    dr1Stream_printf( &ctx->ios, "%s%s", (i?",":""), 
		    autos->merc->itemStore.items[i]->name);
	}
	dr1Stream_printf( &ctx->ios, ")\nBuy what:");
ENTRY(GETWHAT)
	if (qgets( autos->_what, sizeof(autos->_what), ctx)) return 1;
	v[2]= autos->_what;
	c = 3;
    }

    autos->b = dr1Merchant_buy( autos->merc, v[2]);
    if (!autos->b) {
	/* no such item */
	dr1Context_popcall( ctx, -1);
	return 0;
    }

    if (autos->b->item->type->stackable) {
	if (c == 3) {
	    dr1Stream_printf( &ctx->ios, "How many would you like to buy?");
ENTRY(GETMANY)
	    if (qgets( autos->_many, sizeof(autos->_many), ctx)) return 1;
	    v[4] = autos->_many;
	} else if (c == 4) {
	} else {
	    dr1Context_popcall( ctx, -1);
	    return 0;
	}
	many = atoi( v[4]);
	if (many < 0) {
	    dr1Context_popcall( ctx, -3);
	    return 0;
	}
	if (many > 1000) {
	    dr1Context_popcall( ctx, -4);
	    return 0;
	}

	dr1Money_compound( &autos->b->start, 
		(float)many, autos->merc->ep);
	dr1Money_compound( &autos->b->minsale, 
		(float)many, autos->merc->ep);
	autos->b->item->count = many;
    } else {
	if (c != 3) {
	    dr1Context_popcall( ctx, -1);
	    return 0;
	}
    }

    autos->offr = dr1Barter_startingOffer( autos->b);
    dr1Money_format( &p->purse, mbuf);
    dr1Stream_printf( &ctx->ios, "You have %s\n", mbuf);
    dr1Stream_printf( &ctx->ios, "Buying %s\n", v[2]);

    do {
/*      printf("mood %d\n", autos->merc->mood); /**/
	dr1Money_format( &autos->b->start, mbuf);
	dr1Stream_printf( &ctx->ios, "%s %s\n", autos->offr, mbuf);
	dr1Stream_printf( &ctx->ios, "You offer? ");

ENTRY(GETOFFER)
	if (qgets( buf, sizeof(buf), ctx)) return 1;

	{ 
	    dr1Money val;
	    dr1Money_parse( &val, buf);
	    autos->offr = dr1Barter_offer( autos->b, val);
	}

	buy = dr1Money_value( &val, autos->merc->ep);
	sell = dr1Money_value( &autos->b->start, autos->merc->ep);
    } while (buy < sell && buy != 0);

    if (buy == 0) {
	dr1Stream_printf( &ctx->ios, "%s\n", (char *) autos->offr);
	dr1Context_popcall( ctx, 0);
	return 0;
    } else {
	if ( dr1Money_deduct( &p->purse, &autos->b->start)) {
	    dr1Stream_printf( &ctx->ios, "Sorry, you haven't that much in your purse.\n");
	} else {
	    dr1Stream_printf( &ctx->ios, "%s\n", (char *) autos->offr);
	    dr1Merchant_completeSale( autos->merc, autos->b);
	    dr1ItemSet_add( &p->pack, autos->b->item);
	}
	free(autos->b);
	autos->b = NULL;
	dr1Context_popcall( ctx, 0);
	return 0;
    }
    return 0;
}

int equip( dr1Context *ctx, int c, char **v) {
    /* equip armor, weapons, etc. */
    dr1Item *item;
    dr1Item *unequip = NULL;
    dr1Player *p = &ctx->player;

    if (c != 2) return -1;

    item = dr1ItemSet_findName( &p->pack, v[1]);
    if (!item) return -1;

    if (item->weapon) {
        dr1Weapon *wnew = (dr1Weapon*)item;

	if (wnew->min_str > p->curr_attr._str) {
	    dr1Stream_printf( &ctx->ios, "Sorry, you aren't strong enough to use that weapon.\n");
	    return -2;
	}

	if (wnew->super.type->code == DR1W_MISSILE) {
	    unequip = p->gauche;
	    p->gauche = &wnew->super;
	} else {
	    if (p->weapon) {
		unequip = &p->weapon->super;
		p->weapon = NULL;
	    }
	    p->weapon = wnew;
	}
    } else if ( dr1Item_isArmor( item)) {
        if (p->armor) {
	    unequip = &p->armor->super;
	    p->armor = NULL;
	}   
	p->armor = (dr1Armor*)item;
    } else return -2;

    dr1ItemSet_remove( &p->pack, item);
          
    if (unequip) {
	dr1Stream_printf( &ctx->ios, "Putting %s back in your pack.\n", unequip->name);
	dr1ItemSet_add( &p->pack, unequip);
    }
    dr1Stream_printf( &ctx->ios, "Equipping %s.\n", item->name);
    
    return 0;
}


int rest( dr1Context *ctx, int c, char **v) {
    dr1Money roomcost = { 0, 2, 0, 0, 0 };
    dr1Player *p = &ctx->player;
    dr1Stream_printf( &ctx->ios, "You've rested for 1 day at 2sp per day for room and board.\n");
    p->wounds --;
    if (p->wounds < 0) p->wounds = 0;
    dr1Money_deduct( &p->purse, &roomcost);
    return 0;
}

int save( dr1Context *ctx, int c, char **v) {
    dr1Player *p = &ctx->player;
    char *fname;
    if (c == 1) {
	fname = ctx->fname;
	c = 2;
    } else {
	if (c != 2) return -1;
	fname = v[1];
    }
    dr1Player_save( p, fname);
    return 0;
}

int dr1Town_showDialog( dr1Context *ctx) {
    struct {
	int state;
	char cmd[100];
	char *cmds[10];
	int r;
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    enum { PROMPT, GETCMD };
    int i;
    dr1Player *p = &ctx->player;

    switch ( autos->state) {
	case PROMPT:
	    if ( strlen( ctx->error)) {
		dr1Stream_printf( &ctx->ios, DR1MSG_310, 0, ctx->error);
		*ctx->error = 0;
	    }
	    if ( autos->r) {
		dr1Stream_printf( &ctx->ios, DR1MSG_310, autos->r, "Last command returned error");
	    }

	    dr1Stream_printf( &ctx->ios, DR1MSG_190, "(buy, equip, sell, rest, hunt, save, quit)\n");
	    autos->state = GETCMD;
	    return 0;

	case GETCMD:
	    if (qgets( autos->cmd, sizeof(autos->cmd), ctx)) return 1;
	    
	    /* tokenize the command */
	    i=0;
	    autos->cmds[0] = strtok( autos->cmd, " \t\n");
	    if (!autos->cmds[0]) {
		autos->state = PROMPT;
		return 0;
	    }
	    while ((autos->cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

	    /* interpret command */
	    autos->r = 0;
	    if ( !strcmp(autos->cmds[0], "buy")) {
		pushbuy( ctx, i, autos->cmds);
                autos->state = PROMPT;
		return 0;
	    } else if ( !strcmp( autos->cmds[0], "equip")) {
		autos->r=equip( ctx, i, autos->cmds);
	    } else if ( !strcmp( autos->cmds[0], "hunt")) {
		dr1Context_popcall( ctx, 0);
		return 0;
	    } else if ( !strcmp( autos->cmds[0], "rest")) {
		autos->r=rest( ctx, i, autos->cmds);
	    } else if ( !strcmp( autos->cmds[0], "save")) {
		autos->r=save( ctx, i, autos->cmds);
	    } else if ( !strcmp( autos->cmds[0], "quit")) {
	        dr1Context_popcall( ctx, 'x');
		return 0;
	    } else {
	        dr1Context_error( ctx, "Unknown command: '%s'\n", 
			autos->cmds[0]);
	    }
	    autos->state = PROMPT;
	    return 0;
    } /* switch */
  
    /* save player with new item */
    dr1Player_save( p, ctx->fname);
    
    return 0;
} // main
