#ifndef __DR1CONTEXT__H
#define __DR1CONTEXT__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif
#include <stdio.h>
#include "player.h"
#include "merchant.h"
#include "dr1.h"

/*-------------------------------------------------------------------
 * dr1Context
 *
 *    Represents the player context.
 */
struct dr1Context;


/*-------------------------------------------------------------------
 * dr1CmdSet
 *
 *    The Player structure is the top level structure for all Attributes
 *    of a Player character.
 */

typedef int (handler)(struct dr1Context *ctx, int argc, char **argv);
typedef struct dr1CmdSet {
    char *cmds;			/* list of commands to accept */
    handler *h;			/* handler for the command set */
} dr1CmdSet;

#define MAXNSETS 20
typedef struct dr1CmdState {
    int n;			/* number of command sets active */
    dr1CmdSet* sets[MAXNSETS];	/* pointers to active command sets */
} dr1CmdState;

/*-------------------------------------------------------------------
 * enums
 *
 */
enum { START_MU, START_THIEF, START_CLERIC, START_FIGHTER, START_MAX };

struct dr1StartingValues {
    /* starting values per class; reset with each reroll */
    int hits[START_MAX];
    dr1Money gold[START_MAX];
    int estr;
};

struct dr1CombatState {
    int surprise;		/* 1 if player surprised, 0 not surprised */
};


struct dr1PurchaseState {
    char _who[80];
    char _what[80];
    char _many[80];
    dr1Merchant *merc;
    char* offr;
    dr1Barter* b;
};

typedef struct dr1Context {

    /* PERSISTENT STATE */

    char fname[80];		/* players xml character file */
    char login[80];
    char passwd[80];
    dr1Player player;		/* the loaded player */
    dr1Map *map;		/* the active map */

    /* TRANSIENT STATE */

    int loggedin;		/* true if logged in */
    dr1Stream ios;		/* stream connected to the player */
    char error[80];		/* error string */
    int inputready;		/* the socket is readable */

    /* Character creation state */
    struct dr1StartingValues creationstate; 	
				/* prerolls for each possible class */

    /* Combat state */
    struct dr1CombatState combatstate;

    /* Purchase state */
    struct dr1PurchaseState purchasestate;

    /* Command State */
    struct dr1CmdState cmds;    /* command handlers for current context */
} dr1Context;

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
int dr1Context_save( dr1Context *ctx);

/*-------------------------------------------------------------------
 * dr1Context_load
 *
 *    Load player context from disk
 *
 *  PARAMETERS:
 *    ctx     Context to fill
 *    fname   File name to load from
 *
 *  RETURNS:
 *    0 success
 *    1 failure
 */
int dr1Context_load( dr1Context *ctx, char *fname);

/*-------------------------------------------------------------------
 * dr1Context_destroy
 *
 *    Destroy a malloc'd dr1Context structure.
 */
void dr1Context_destroy( dr1Context *ctx);

/*-------------------------------------------------------------------
 * dr1Contect_resetCreationState
 *
 *    Destroy a malloc'd dr1Context structure.
 */
void dr1Context_resetCreationState( dr1Context *ctx);

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
 * dr1Context_enable
 *
 *    Enables a set of commands.  New incoming commands that match
 *    the supplied command list will be passed to the supplied 
 *    handler.  
 *
 *  PARAMETERS:
 *    dr1CmdSet* set	Set of commands to enable
 *
 *  RETURNS:
 */
void dr1Context_enable( dr1Context *ctx, dr1CmdSet* set);
void dr1Context_disable( dr1Context *ctx, dr1CmdSet* set);

/*-------------------------------------------------------------------
 * dr1Context_handle
 *
 *    Handle a command.
 *
 *  PARAMETERS:
 *    int argc		Number of arguments
 *    char **argv	Values of the arguments
 *
 *  RETURNS:
 *    <0		Error processing command
 *    0			All ok.
 *    
 */
int dr1Context_handle( dr1Context *ctx, int argc, char **argv);

#endif /* __DR1CONTEXT__H */
