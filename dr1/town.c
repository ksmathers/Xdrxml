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

struct cmdargs_t {
    int c;
    char **v;
};

int dobuy( dr1Context *ctx);
int pushbuy( dr1Context *ctx, int c, char **v) {
    struct cmdargs_t *args = dr1Context_pushcallv( ctx, dobuy, 
	    "cmdargs_t", sizeof(*args));
    args->c = c;
    args->v = v;
    return 0;
}
int dobuy( dr1Context *ctx) {
    struct cmdargs_t *args = dr1Context_args( ctx, "cmdargs_t");
    struct {
	int state;
	char _who[80];
	char _what[80];
	char _many[80];
	dr1Merchant *merc;
	char* offr;
	dr1Barter* b;
    } *autos = dr1Context_auto( ctx, sizeof(*autos));

    /* purchase an item */
    dr1Player *p = &ctx->player;
    dr1Money val;
    long sell, buy;
    char mbuf[ 40];
    char buf[ 100];
    int many;
    int i;
    enum { SHOWWHO, GETWHO, SHOWWHAT, GETWHAT, SHOWMANY, GETMANY, BUY, BUY1,
    PROMPTOFFER, GETOFFER, ABORT, DONE };

    switch (autos->state) {
	case SHOWWHO:
	    if (args->c == 1) {
	        qprintf( ctx, "Shops in Dragon's Reach:\n");
		qprintf( ctx, " - Fnord the Apothecary\n");
		qprintf( ctx, " - Gandor the Blacksmith\n");
		qprintf( ctx, " - John Frommer's Leathers\n");
		qprintf( ctx, " - John the Wheel Wright\n\n");
		qprintf( ctx, "(fnord, gandor, frommer, wright)\n");
		qprintf( ctx, "Buy from whom:");
		autos->state = GETWHO;
	    } else autos->state = SHOWWHAT;
	    return 0;

	case GETWHO:
	    if (qgets( autos->_who, sizeof(autos->_who), ctx)) return 1;
	    args->v[1]= autos->_who;
	    args->c = 2;

	case SHOWWHAT:
	    if ( !strcasecmp( args->v[1], "Fnord")) {
		autos->merc = &dr1apothecary;
	    } else if ( !strcasecmp( args->v[1], "Gandor")) {
		autos->merc = &dr1smithy;
	    } else if ( !strcasecmp( args->v[1], "Frommer")) {
		autos->merc = &dr1tanner;
	    } else if ( !strcasecmp( args->v[1], "Wright")) {
		autos->merc = &dr1wright;
	    } else {
		dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    qprintf( ctx, "Welcome to '%s'\n", autos->merc->store);
	    if (autos->merc->mood > 10) {
		qprintf( ctx, "%s looks a bit peeved today.\n", 
			autos->merc->name);
	    }
	    if (args->c == 2) {
		qprintf( ctx, "(");
		for (i=0; i<autos->merc->itemStore.len; i++) {
		    qprintf( ctx, "%s%s", (i?",":""), 
			    autos->merc->itemStore.items[i]->name);
		}
		qprintf( ctx, ")\nBuy what:");
		autos->state = GETWHAT;
	    } else {
		autos->state = SHOWMANY;
	    }
	    return 0;
	
	case GETWHAT:
	    if (qgets( autos->_what, sizeof(autos->_what), ctx)) return 1;
	    args->v[2]= autos->_what;
	    args->c = 3;

        case SHOWMANY:
	    autos->b = dr1Merchant_buy( autos->merc, args->v[2]);
	    if (!autos->b) {
		/* no such item */
		dr1Context_popcall( ctx, -1);
		return 0;
	    }

	    if (autos->b->item->type->stackable) {
		if (args->c == 3) {
		    qprintf( ctx, "How many would you like to buy?");
		    autos->state = GETMANY;
		} else if (args->c == 4) {
		    autos->state = BUY;
		} else {
		    dr1Context_popcall( ctx, -1);
		}
	    } else {
		if (args->c != 3) {
		    dr1Context_popcall( ctx, -1);
		} else {
		    autos->state = BUY1;
		}
	    }
	    return 0;

	case GETMANY:
	    qgets( autos->_many, sizeof(autos->_many), ctx);
	    args->v[4] = autos->_many;
	    autos->state = BUY;

	case BUY:
	    many = atoi( args->v[4]);
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
	    autos->state = BUY1;

	case BUY1:
	    autos->offr = dr1Barter_startingOffer( autos->b);
	    dr1Money_format( &p->purse, mbuf);
	    qprintf( ctx, "You have %s\n", mbuf);
	    qprintf( ctx, "Buying %s\n", args->v[2]);
	    autos->state = PROMPTOFFER;

	case PROMPTOFFER:
/*            printf("mood %d\n", autos->merc->mood); /**/
	    dr1Money_format( &autos->b->start, mbuf);
	    qprintf( ctx, "%s %s\n", autos->offr, mbuf);
	    qprintf( ctx, "You offer? ");
	    autos->state = GETOFFER;

	case GETOFFER:
	    if (qgets( buf, sizeof(buf), ctx)) return 1;
	    dr1Money_parse( &val, buf);
	    autos->offr = dr1Barter_offer( autos->b, val);

	    buy = dr1Money_value( &val, autos->merc->ep);
	    sell = dr1Money_value( &autos->b->start, autos->merc->ep);
	    if (buy >= sell) autos->state = DONE;
	    else if (buy == 0) autos->state = ABORT;
	    else autos->state = PROMPTOFFER;
	    return 0;

	case ABORT:
	    qprintf( ctx, "%s\n", (char *) autos->offr);
	    dr1Context_popcall( ctx, 0);
	    return 0;

	case DONE:
	    if ( dr1Money_deduct( &p->purse, &autos->b->start)) {
		qprintf( ctx, "Sorry, you haven't that much in your purse.\n");
	    } else {
		qprintf( ctx, "%s\n", (char *) autos->offr);
		dr1Merchant_completeSale( autos->merc, autos->b);
		dr1ItemSet_add( &p->pack, autos->b->item);
	    }
	    free(autos->b);
	    autos->b = NULL;
	    dr1Context_popcall( ctx, 0);
	    return 0;
    } /* switch */
    return -1;
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
	    qprintf( ctx, "Sorry, you aren't strong enough to use that weapon.\n");
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
	qprintf( ctx, "Putting %s back in your pack.\n", unequip->name);
	dr1ItemSet_add( &p->pack, unequip);
    }
    qprintf( ctx, "Equipping %s.\n", item->name);
    
    return 0;
}


int rest( dr1Context *ctx, int c, char **v) {
    dr1Money roomcost = { 0, 2, 0, 0, 0 };
    dr1Player *p = &ctx->player;
    qprintf( ctx, "You've rested for 1 day at 2sp per day for room and board.\n");
    p->wounds --;
    if (p->wounds < 0) p->wounds = 0;
    dr1Money_deduct( &p->purse, &roomcost);
    return 0;
}

int save( dr1Player *p, int c, char **v) {
    char *fname;
    if (c == 1) {
	fname="player.dat";
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
	    qprintf( ctx, "---------------------------------------------------------\n");
	    if (p->name) qprintf( ctx, "Name: %s\n", p->name);

	    if ( strlen( ctx->error)) {
		qprintf( ctx, "--\n: %s\n--\n", ctx->error);
		*ctx->error = 0;
	    }
	    if ( autos->r) {
		qprintf( ctx, "Command returned code %d\n", autos->r);
	    }

	    qprintf( ctx, "(buy, equip, sell, rest, hunt, save, quit)\n");
	    qprintf( ctx, "Command: ");
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
	    } else if ( !strcmp( autos->cmds[0], "rest")) {
		autos->r=rest( ctx, i, autos->cmds);
	    } else if ( !strcmp( autos->cmds[0], "save")) {
		autos->r=save( p, i, autos->cmds);
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
