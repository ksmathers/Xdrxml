#include <stdio.h>
#include "money.h"
#include "dice.h"

void dump( char *s, dr1Money p) {
    printf("[%s] %dcp, %dsp, %dep, %dgp, %dpp\n", s, p.cp, p.sp, p.ep, p.gp, p.pp);
}

int main(int argc, char **argv) {
    int i,j;
    char *dice[] = { "d8", "d10", "d100", "dr1", "d4", "3d6", 
        "2d4+1", "dr1-4" };

    dr1Money p = { 
	    200 /* cp */, 
	    50 /* sp */, 
	    60 /* ep */, 
	    20 /* gp */,
	    10 /* pp */ 
	};

    dr1Money chg, tp;
    XDR xdrs;
    FILE *fp;
    dr1Dice_seed();

    dump( "original", p);
    tp=p;
    
    chg = dr1Money_normalizex( &p, 0.1, 1);

    dump( "normalize", p);
    dump( "charge", chg);

    dr1Money_add( &p, &chg);
    dump( "combined", p);
    dr1Money_normalizex( &tp, 0.0, 1);
    dump( "equivalent", tp);

    dr1Money_deductx( &tp, &chg, 1);
    dump( "chg deduct", tp);

    fp = fopen("money.dat", "w");
    xdrstdio_create( &xdrs, fp, XDR_ENCODE);
    xdr_dr1Money( &xdrs, &p);
    xdr_destroy( &xdrs);
    fclose( fp);
    
    fp = fopen("money.dat", "r");
    xdrstdio_create( &xdrs, fp, XDR_DECODE);
    xdr_dr1Money( &xdrs, &p);
    xdr_destroy( &xdrs);
    fclose( fp);
    dump( "combined (reload)", p);
    
    for (j=0; j<sizeof(dice)/sizeof(*dice); j++) {
	printf("%s\t", dice[j]);
    }
    printf("\n");
    for (i=0; i<10; i++) {
        for (j=0; j<sizeof(dice)/sizeof(*dice); j++) {
	    printf("%d\t", dr1Dice_roll(dice[j]));
	}
	printf("\n");
    }
    return 0;
}
