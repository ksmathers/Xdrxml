#ifndef __DR1CONTEXT__H
#define __DR1CONTEXT__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include <stdio.h>
#include "player.h"
#include "map.h"
#define CMDSTACK_SIZE 20

/*-------------------------------------------------------------------
 * dr1Context
 *
 *    Represents the player context.
 */
struct dr1Context;
typedef int (*dr1Command_fnp)(struct dr1Context *ctx);

typedef struct {
    dr1Command_fnp cmd;
    void *auto_data;
    void *args;
    char *argtype;
    int result;
} dr1Cmd;

typedef struct dr1Context {
    FILE *fp;			/* socket connected to the player */
    char fname[80];		/* players xml character file */
    char error[80];		/* error string */
    dr1Player player;		/* the loaded player */
    dr1Map *map;		/* the active map */
    int state;			/* current dialog state */
    int inputready;		/* the socket is readable */
    dr1Cmd cstack[CMDSTACK_SIZE];	/* command stack */
    int stackptr;
} dr1Context;

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
int dr1Context_return( dr1Context *ctx);

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
void dr1Context_error( dr1Context *ctx, char *fmt, ...);

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
void* dr1Context_auto( dr1Context *ctx, size_t size);

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
void* dr1Context_args( dr1Context *ctx, char *argtype);

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
 *    NULL failure
 *    or pointer to newly allocated arglist
 *
 *  SIDE EFFECTS:
 *   allocates space for the args and returns the new pointer.
 */
void *dr1Context_pushcallv( 
	dr1Context *ctx, 
	dr1Command_fnp cmd,
	char *argtype,
	int argsize
    );

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *    0  success
 *   -1  stack overflow
 *
 *  SIDE EFFECTS:
 */
int dr1Context_pushcall( dr1Context *ctx, dr1Command_fnp cmd);

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
int dr1Context_popcall( dr1Context *ctx, int rcode);
#endif /* __DR1CONTEXT__H */
