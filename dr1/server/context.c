#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "context.h"
#include "dr1.h"

#define DEFAULT_MAP "town.map"

/*-------------------------------------------------------------------
 * dr1Context_save
 *
 *    Saves the player game state 
 *
 *  PARAMETERS:
 *    ctx   The context to save
 *
 *  RETURNS:
 *    0 success
 *    1 error
 */
int dr1Context_save( dr1Context *ctx) {
    /* save player context */
    FILE *fp = NULL;
    char *fname = ctx->fname;
    bool_t res;
    char *cpos;
    XDR xdrs;

    cpos = rindex( fname, '.');
    if (cpos && strcasecmp(cpos, ".xml")==0) {
        printf("Saving xml\n");
	xdr_xml_create( &xdrs, fname, XDR_ENCODE);
    } else {
        printf("Saving binary\n");
	fp = fopen(fname, "w");
	xdrstdio_create( &xdrs, fp, XDR_ENCODE);
    }
    xdr_push_note( &xdrs, "context");

    xdr_push_note( &xdrs, "player");
    res = xdr_dr1Player( &xdrs, &ctx->player);
    xdr_pop_note( &xdrs);

    xdr_push_note( &xdrs, "map");
    res = xdr_dr1Map( &xdrs, ctx->map);
    xdr_pop_note( &xdrs);

    xdr_pop_note( &xdrs);
    if (!res) {
	printf("Error saving to '%s'\n", fname);
    }
    xdr_destroy( &xdrs);
    if (fp) fclose( fp);
    return !res;
}


/*-------------------------------------------------------------------
 * dr1Context_load
 *
 *    Load player context from disk
 *
 *  PARAMETERS:
 *    fname   File name to load from
 *
 *  RETURNS:
 *    0   success
 *    1   failure
 */

int dr1Context_load( dr1Context *ctx, char *fname) {
    FILE *fp = NULL;
    bool_t ok;
    XDR xdrs;
    char *cpos;

    cpos = rindex( fname, '.');
    if (cpos && strcasecmp(cpos, ".xml")==0) {
/*        printf("Loading xml\n"); /**/
	xdr_xml_create( &xdrs, fname, XDR_DECODE);
    } else {
/*        printf("Loading binary\n"); /**/
	fp = fopen( fname, "r");
	if (!fp) return 1;

	xdrstdio_create( &xdrs, fp, XDR_DECODE);
    }

    assert(ctx);

    xdr_push_note( &xdrs, "context");

    xdr_push_note( &xdrs, "player");
    ok = xdr_dr1Player( &xdrs, &ctx->player);
    xdr_pop_note( &xdrs);

    xdr_push_note( &xdrs, "map");
    ctx->map = calloc( 1, sizeof(dr1Map));
    ok = xdr_dr1Map( &xdrs, ctx->map);
    if (!ok) {
	/* save file missing a map, load the default map instead. */
	ctx->map = dr1Map_readmap( "lib/maps/town.map");
	ok = 1; 
    }
    xdr_pop_note( &xdrs);

    xdr_pop_note( &xdrs);
    xdr_destroy( &xdrs);
    if (fp) fclose( fp);
      
    if (!ok) {
	dr1Context_destroy( ctx);
	ctx = NULL;
    }
    return (ctx!=NULL);
}

/*-------------------------------------------------------------------
 * dr1Context_destroy
 *
 *    Destroy a malloc'd dr1Context structure.
 */
void dr1Context_destroy( dr1Context *ctx) {
    xdr_free( (xdrproc_t)xdr_dr1Player, (void*)&ctx->player);
    xdr_free( (xdrproc_t)xdr_dr1Map, (void*)ctx->map);
    free( ctx->map);
    ctx->map = NULL;
}

/*-------------------------------------------------------------------
 * dr1Contect_resetCreationState
 *
 *    Destroy a malloc'd dr1Context structure.
 */
void dr1Context_resetCreationState( dr1Context *ctx) {
    bzero( &ctx->creationstate, sizeof(ctx->creationstate));
}

/*-------------------------------------------------------------------
 * dr1Context_error
 *
 *    Sets the error message from a failed subroutine
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 *    ctx->error is the formatted error string
 */
void dr1Context_error( dr1Context *ctx, char *fmt, ...) {
    va_list va;
    va_start( va, fmt);
    vsnprintf( ctx->error, sizeof(ctx->error), fmt, va);
}
