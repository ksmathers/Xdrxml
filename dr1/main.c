#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "player.h"
#include "apothecary.h"
#include "money.h"
#include "barter.h"
#include "dice.h"
#include "xdrasc.h"

int main( int argc, char** argv) {
    FILE *fp;
    XDR xdrs;
    char buf[ 100];
    char mbuf[ 40];
    int len;
    dr1Player player;
    dr1Money val;
    dr1Barter* b;
    char* offr;
    long sell, buy;
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
    printf("class=%08x\n", player.class);
    }

    /* purchase a potion */
    b = dr1Merchant_buy( &dr1apothecary, "Healing Potion");
    offr = dr1Barter_startingOffer( b);
    printf("You have %dgp\n", player.purse.gp);
    printf("Buying Healing Potion\n");
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
    } else if ( dr1Money_deduct( &player.purse, &b->start)) {
	printf("Sorry, you haven't that much in your purse.\n");
    } else {
	printf("%s\n", (char *) offr);
	dr1Merchant_completeSale( &dr1apothecary, b);
	dr1ItemSet_add( &player.pack, b->item);
    }
    free(b);
  
    printf("class=%08x\n", player.class);
    /* dump player */
    printf("Player\n");
    xdr_push_note( &xdrasc, "player");
    xdr_dr1Player( &xdrasc, &player);
    xdr_pop_note( &xdrasc);

    printf("class=%08x\n", player.class);
    /* save player with new item */
    fp = fopen("player.dat", "w");
    xdrstdio_create( &xdrs, fp, XDR_ENCODE);
    xdr_dr1Player( &xdrs, &player);
    xdr_destroy( &xdrs);
    fclose( fp);
    
    return 0;
} // main
