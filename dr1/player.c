#include <stdio.h>
#include <string.h>

#include "player.h"


/*-------------------------------------------------------------------
 * dr1Player_load
 *
 *    Load a player object from disk
 *
 *  PARAMETERS:
 *    fn   Filename to be loaded
 *
 *  RETURNS:
 *    Pointer to a malloc'd dr1Player structure, or NULL if there
 *    was a problem loading the file.
 */
dr1Player *dr1Player_load( dr1Player *buf, char* fname) {
    FILE *fp;
    bool_t fail;
    dr1Player *p;
    XDR xdrs;

    fp = fopen( fname, "r");
    if (!fp) return NULL;

    if (buf) {
        p = buf;
	bzero( p, sizeof(dr1Player));
    } else {
	p = calloc(1,sizeof(dr1Player));
    }

    xdrstdio_create( &xdrs, fp, XDR_DECODE);
    fail = xdr_dr1Player( &xdrs, p);
    xdr_destroy( &xdrs);
    fclose( fp);
      
    if (fail) {
	dr1Player_destroy( p);
	p = NULL;
    }
    return p;
}

/*-------------------------------------------------------------------
 * dr1Player_destroy
 *
 *    Destroy a malloc'd dr1Player structure.
 */
void dr1Player_destroy( dr1Player *p) {
    free( p->name);
/*    dr1ItemSet_destroy( &p->pack); /**/
}

/*-------------------------------------------------------------------
 * dr1Player_encumbrance
 *
 *    Calculates the encubrance that the Player is carrying in pounds.
 */

int dr1Player_encumbrance( dr1Player *p) {
    int lbs;
    lbs = dr1Money_encumbrance( &p->purse);
    return lbs;
}

/*-------------------------------------------------------------------
 * xdr_dr1Player( xdrs, dr1Player*)
 */
bool_t xdr_dr1Player( XDR *xdrs, dr1Player* p) {
   bool_t res;
   res = xdr_string( xdrs, &p->name, MAXNAMELEN );
   res |= xdr_long( xdrs, &p->xp);
   res |= xdr_int( xdrs, &p->hp);
   res |= xdr_int( xdrs, &p->full_hp);
   res |= xdr_dr1Attr( xdrs, &p->base_attr);
   res |= xdr_dr1Attr( xdrs, &p->curr_attr);
   res |= xdr_dr1Money( xdrs, &p->purse);
   res |= xdr_dr1ItemSet( xdrs, &p->pack);
   return res;
}

