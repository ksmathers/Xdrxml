#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "context.h"
#include "lib/xdrxml.h"

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
 * dr1Context_return
 *
 *    Sets the error message from a failed subroutine
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    The return code of the function that just popcall'd
 */
int dr1Context_return( dr1Context *ctx) {
    return ctx->cstack[ctx->stackptr-1].result;
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

/*-------------------------------------------------------------------
 * dr1Context_auto
 *
 *    Allocates automatic space on the command stack
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    Pointer to auto data (newly calloc'd if needed)
 *
 *  SIDE EFFECTS:
 */
void* dr1Context_auto( dr1Context *ctx, size_t size) {
    void **a;
    a = &ctx->cstack[ctx->stackptr-1].auto_data; 
    if (!*a) *a = calloc( 1, size);
    return *a;
}

/*-------------------------------------------------------------------
 * dr1Context_args
 *
 *    Returns pointer to the arguments for the current running
 *    function.
 *
 *  PARAMETERS:
 *    ctx   Context 
 *
 *  RETURNS:
 *    Pointer to argument list
 *
 *  SIDE EFFECTS:
 */
void* dr1Context_args( dr1Context *ctx, char *argtype) {
    dr1Cmd *c = &ctx->cstack[ ctx->stackptr-1];
    assert( !strcmp( c->argtype, argtype));
    return c->args;
}

/*-------------------------------------------------------------------
 * dr1Context_pushcallv
 *
 *    The method dr1Context_pushcall adds a command to the command
 *    stack.
 *
 *  PARAMETERS:
 *    ctx     Context to push the call into
 *    cmd     The command to be pushed
 *    argtype Argument type
 *    argsize Size of argument list
 *
 *  RETURNS:
 *    NULL stack overflow
 *    pointer to newly allocated args
 *
 *  SIDE EFFECTS:
 *   allocates space for the args and 
 */
void *dr1Context_pushcallv( 
	dr1Context *ctx, 
	dr1Command_fnp cmd,
	char *argtype,
	int argsize) 
{
    dr1Cmd *c;
    if (ctx->stackptr >= CMDSTACK_SIZE) return NULL;
    c = &ctx->cstack[ ctx->stackptr++];
    if ( argsize) {
	c->args = calloc( 1, argsize);
	c->argtype = argtype;
    }
    c->cmd = cmd;

    printf("%d: %p pushed with args %p\n", fileno(ctx->fp), cmd, c->args); /**/
    return c->args;
}

/*-------------------------------------------------------------------
 * dr1Context_pushcall
 *
 *    The method dr1Context_pushcall adds a command to the command
 *    stack.
 *
 *  PARAMETERS:
 *    ctx   Context to push the call into
 *    cmd   The command to be pushed
 *
 *  RETURNS:
 *    0  success
 *   -1  stack overflow
 *
 *  SIDE EFFECTS:
 */
int dr1Context_pushcall( dr1Context *ctx, dr1Command_fnp cmd) {
    if (ctx->stackptr >= CMDSTACK_SIZE) return -1;
    ctx->cstack[ ctx->stackptr++].cmd = cmd;

    printf("%d: %p pushed\n", fileno(ctx->fp), cmd); /**/
    return 0;
}

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    0  success
 *   -1  stack underflow
 *
 *  SIDE EFFECTS:
 */
int dr1Context_popcall( dr1Context *ctx, int rcode) {
    dr1Cmd *c;
    if (ctx->stackptr <= 0) return -1;
    c = &ctx->cstack[--ctx->stackptr];

    printf("%d: %p returns %d\n", 
	fileno(ctx->fp), ctx->cstack[ctx->stackptr].cmd, rcode);

    /* clean up the call stack */
    if (c->auto_data) { free(c->auto_data); c->auto_data = NULL; }
    if (c->args) { free( c->args); c->args = NULL; }
    c->argtype = NULL;

    /* set return code */
    ctx->cstack[ctx->stackptr-1].result = rcode;
    return 0;
}
