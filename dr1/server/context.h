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
typedef int (*dr1Command_fnp)(struct dr1Context *ctx);

typedef struct {
    dr1Command_fnp cmd;
    void *auto_data;
    void *args;
    char *argtype;
    int result;
} dr1Cmd;

enum { START_MU, START_THIEF, START_CLERIC, START_FIGHTER, START_MAX };
enum states { LOGIN, NEWPLAYER, TOWN, DUNGEON };
enum dialog { ACTIVE, DONE, CANCEL };

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
    int state;
    char _who[80];
    char _what[80];
    char _many[80];
    dr1Merchant *merc;
    char* offr;
    dr1Barter* b;
};

typedef struct dr1Context {
    dr1Stream ios;		/* stream connected to the player */
    char fname[80];		/* players xml character file */
    char error[80];		/* error string */
    char login[80];
    char passwd[80];
    dr1Player player;		/* the loaded player */
    dr1Map *map;		/* the active map */
    int state;			/* current protocol */
    int dialog;			/* dialog completion status */
    int inputready;		/* the socket is readable */

    /* Character creation state */
    struct dr1StartingValues creationstate; 	
				/* prerolls for each possible class */

    /* Combat state */
    struct dr1CombatState combatstate;

    /* Purchase state */
    struct dr1PurchaseState purchasestate;
    
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

#endif /* __DR1CONTEXT__H */
