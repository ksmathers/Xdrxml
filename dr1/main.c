#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "player.h"
#include "playerv.h"
#include "apothecary.h"
#include "smithy.h"
#include "money.h"
#include "barter.h"
#include "dice.h"
#include "xdrasc.h"

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
    int len;

    if (c != 3) return -1;

    who = v[1];
    what = v[2];

    if ( !strcasecmp( who, "Fnord")) merc = &dr1apothecary;
    else if ( !strcasecmp( who, "Gandor")) merc = &dr1smithy;
    else return -1;
    
    b = dr1Merchant_buy( merc, what);
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

    /* restore player from disk */
    pdatf = stat( "player.dat", &pdat);

    if (!pdatf && S_ISREG(pdat.st_mode)) {
	loadOk = dr1Player_load( &player, "player.dat");
	assert(loadOk);
    } else {
        dr1Playerv_showDialog( &player);
    }

    
    {
	char *v[3] = { "buy", "Fnord", "Healing Potion" };
	buy( &player, 3, v);
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
