#ifndef __CONTROLLER__H
#define __CONTROLLER__H

#include "context.h"
/*
 * sendmap 
 *
 */
void sendmap( dr1Context* ctx); 

/*
 * sendplayer 
 *
 */
void sendplayer( dr1Context* ctx); 


/*-------------------------------------------------------------------
 * setLoginPassword
 *
 *    Puts the login and password into the player context, and
 *    checks to see if they represent a current user
 *
 *  PARAMETERS:
 *    ctx    Player context
 *    login  Login name
 *    passwd Password of the user
 *
 *  RETURNS:
 *    0 - User currently exists
 *    1 - User doesn't exist
 *
 *  SIDE EFFECTS:
 */
int 
setLoginPassword(dr1Context *ctx, char *login, char *passwd);

/*--------------------------------------------------------------------------
 * dr1Controller_handleLogin
 *
 *     Runs through the login sequence.
 *
 *     This is one of several input handling routines that returns 
 *     immediately each time it is called.  
 *
 *   Parameters:
 *     ctx    Player context
 *
 *   Returns:
 *     0      OK
 *     1      Unrecoverable Protocol error or Socket error
 *
 *   Side Effects:
 *     ctx is changed to reflect the state of the interaction with the user
 *     data written to the player socket
 */
int dr1Controller_handleLogin( dr1Context *ctx, int argc, char **argv); 
	
int
dr1Controller_handleNewPlayer( dr1Context *ctx, int argc, char **argv); 

int savegame( dr1Context *ctx); 

int domove( dr1Context *ctx, int argc, char **argv); 

#if 0
struct {
} cmdsets = {
    loginplayer, { "iam" },
    createplayer, { "trade", "swap", "name", "class", "sex", "race", "done" },
    town, { "buy", "sell", "equip", "talk", "move", "use", "enter", "rent" },
    dungeon, { "equip", "talk", "attack", "move", "use", "enter" },
    dead, { "quit" }
};
#endif

/*-------------------------------------------------------------------
 * dr1Controller_handleCommand
 *
 *    The method checks the current command against the player connection
 *    state, and passes it off to the appropriate subhandler.
 *
 *    There are subhandlers for:
 *      login
 *      player generation
 *      city commands (buying and selling, rent)
 *      dungeon commands (combat, melee movement)
 *      game commands (non-melee movement, logout, talk)
 *
 *  PARAMETERS:
 *    ctx   Player context
 *    argc  Number of command args
 *    argv  Array of commands
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
int dr1Controller_handleCommand( dr1Context *ctx, int argc, char **argv); 

#endif /* __CONTROLLER__H */
