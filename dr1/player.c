#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "player.h"
#include "xdrasc.h"

#if 0
struct dr1Player_state {
    int state; 
    char *select;
    dr1Player_select_fp doSelect;
    int nextstate;
}

enum { PC1_START, PC1_STATS, PC1_RACE, PC1_SEX, PC1_CLASS, PC1_ALIGN, PC1_NAME };
struct dr1Player_state[] dr1Player_create_method1 = {
	{ PC1_START, "roll", dr1Player_roll_method1, PC1_STATS },
	{ PC1_START, "cancel", dr1Player_cancel, -1 },
	{ PC1_STATS, "swap", dr1Player_swap, PC1_STATS },
        { PC1_STATS, "trade", dr1Player_trade, PC1_STATS },
	{ PC1_STATS, "done", dr1Player_done, PC1_RACE },
	{ PC1_STATS, "cancel", dr1Player_cancel, -1 }
    };

struct dr1Player_stat[] dr1Player_create_cont = {
	{ PC1_RACE, "race", dr1Player_race, PC1_SEX },
	{ PC1_SEX, "sex", dr1Player_sex, PC1_CLASS },
	{ PC1_CLASS, "class", dr1Player_class, PC1_ALIGN },
	{ PC1_ALIGN, "align", dr1Player_align, PC1_NAME },
	{ PC1_NAME, "name", dr1Player_align, -1 }
    };
#endif

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
    bool_t ok;
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
    ok = xdr_dr1Player( &xdrs, p);
    xdr_destroy( &xdrs);
    fclose( fp);
      
    if (!ok) {
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
    xdr_free( (xdrproc_t)xdr_dr1Player, (void*)p);
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
   xdr_attr( xdrs, "name");
   if (!xdr_wrapstring( xdrs, &p->name)) return FALSE;

   xdr_attr( xdrs, "xp");
   if (!xdr_long( xdrs, &p->xp)) return FALSE;

   xdr_attr( xdrs, "hp");
   if (!xdr_int( xdrs, &p->hp)) return FALSE;

   xdr_attr( xdrs, "full_hp");
   if (!xdr_int( xdrs, &p->full_hp)) return FALSE;

   xdr_push_note( xdrs, "base_attr");
   if (!xdr_dr1Attr( xdrs, &p->base_attr)) return FALSE;
   xdr_pop_note( xdrs);

   xdr_push_note( xdrs, "curr_attr");
   if (!xdr_dr1Attr( xdrs, &p->curr_attr)) return FALSE;
   xdr_pop_note( xdrs);

   xdr_push_note( xdrs, "purse");
   if (!xdr_dr1Money( xdrs, &p->purse)) return FALSE;
   xdr_pop_note( xdrs);

   xdr_push_note( xdrs, "pack");
   if (!xdr_dr1ItemSet( xdrs, &p->pack)) return FALSE;
   xdr_pop_note( xdrs);

   xdr_attr( xdrs, "race");
   if (!xdr_int( xdrs, &p->race)) return FALSE;

   xdr_attr( xdrs, "sex");
   if (!xdr_int( xdrs, &p->sex)) return FALSE;

   xdr_attr( xdrs, "class");
   if (!xdr_int( xdrs, &p->class)) return FALSE;

   return TRUE;
}

