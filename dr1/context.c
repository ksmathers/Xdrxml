#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "context.h"

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
