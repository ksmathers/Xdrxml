#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include "controller.h"
#include "playerv.h"
#include "town.h"

#include "dr1.h"


/*
 * login commands
 */
handler handleLogin;
dr1CmdSet controller_login_cmds = {
    "iam:newplayer",
    handleLogin
};

void dr1Controller_disableLogin( dr1Context *ctx) {
    printf("%d: disable login\n", ctx->ios.fd);
    dr1Context_disable( ctx, &controller_login_cmds);
}
void dr1Controller_enableLogin( dr1Context *ctx) {
    printf("%d: enable login\n", ctx->ios.fd);
    dr1Context_enable( ctx, &controller_login_cmds);
}

/*
 * in game commands 
 */
handler handleGameCmds;
dr1CmdSet controller_game_cmds = {
    "move:save",
    handleGameCmds
};
void dr1Controller_disableGame( dr1Context *ctx) {
    printf("%d: disable game\n", ctx->ios.fd);
    dr1Context_disable( ctx, &controller_game_cmds);
}
void dr1Controller_enableGame( dr1Context *ctx) {
    printf("%d: enable game\n", ctx->ios.fd);
    dr1Context_enable( ctx, &controller_game_cmds);
}

/*
 * sendmap 
 *
 */
void sendmap( dr1Context* ctx) {
    char *buf;
    int res;
    xdrxmlsb_reset( &xdrxmlsb);
    xdr_push_note( &xdrxmlsb, "map");
    if (xdr_dr1Map( &xdrxmlsb, ctx->map) != TRUE) {
	printf("Error serializing map.\n");
    }
    xdr_pop_note( &xdrxmlsb);

    buf = xdrxmlsb_getbuf( &xdrxmlsb); 

    psendMessage( &ctx->ios, DR1MSG_120, 
	    0, 0, ctx->map->xsize, ctx->map->ysize);
    res = dr1Stream_printf( &ctx->ios, buf);
    if (res < 0) { perror( "sendmap.c: printf"); }
    res = dr1Stream_printf( &ctx->ios, "%s\n", SEPARATOR);
    if (res < 0) { perror( "sendmap.c: printf2"); }
}

/*
 * sendplayer 
 *
 */
void sendplayer( dr1Context* ctx) {
    char *buf;
    int res;
    xdrxmlsb_reset( &xdrxmlsb);
    xdr_push_note( &xdrxmlsb, "player");
    if (xdr_dr1Player( &xdrxmlsb, &ctx->player) != TRUE) {
	printf("Error serializing player.\n");
    }
    xdr_pop_note( &xdrxmlsb);

    buf = xdrxmlsb_getbuf( &xdrxmlsb); 

    psendMessage( &ctx->ios, DR1MSG_170);
    res = dr1Stream_printf( &ctx->ios, buf);
    if (res < 0) perror("controller.c:sendplayer");
    res = dr1Stream_printf( &ctx->ios, "%s\n", SEPARATOR);
    if (res < 0) perror("controller.c:sendplayer-2");
}

/*
 * sendlocation
 *
 */
void sendlocation( dr1Context* ctx) {
    dr1Location *l = &ctx->player.location;
    psendMessage( &ctx->ios, DR1MSG_175, l->mapname, l->x, l->y);
}


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
setLoginPassword(dr1Context *ctx, char *login, char *passwd)
{
    int i;
    char *cpos;
    struct stat pdat;
    int pdatf;

    strncpy( ctx->login, login, sizeof(ctx->login));
    strncpy( ctx->passwd, passwd, sizeof(ctx->passwd));

    /* make player name and password into legal filename */
    i = 0;
    for (cpos = ctx->login; *cpos != 0; cpos++, i++) {
        if (i >= sizeof( ctx->fname)-8) break;
	ctx->fname[i] = isalnum(*cpos)?*cpos:'_';
    }
    ctx->fname[i++] = '-';
    for (cpos = ctx->passwd; *cpos != 0; cpos++, i++) {
        if (i >= sizeof( ctx->fname)-5) return 1;
	ctx->fname[i] = isalnum(*cpos)?*cpos:'_';
    }
    ctx->fname[i] = 0;
    strcat( ctx->fname + i, ".xml");

    /* does the player exist? */
    pdatf = stat( ctx->fname, &pdat);
    if (!pdatf && S_ISREG(pdat.st_mode)) {
        /* player data file is found */
	return 0;
    }
    return 1;
}

/*--------------------------------------------------------------------------
 * dologin
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
int dologin( dr1Context *ctx, int argc, char **argv) {

    int loadOk;
    int exists;
    int status=0;

    printf("%d: dologin\n", ctx->ios.fd);

    exists = ! setLoginPassword( ctx, argv[1], argv[2]);

    if (exists) {
        /* player data file is found */
	loadOk = dr1Context_load( ctx, ctx->fname);
	if (!loadOk) {
	    psendMessage(&ctx->ios, DR1MSG_520, ctx->fname);
	    status = 1;
	} else {
	    ctx->loggedin = TRUE;
	    psendMessage(&ctx->ios, DR1MSG_100);
	    sendplayer( ctx);
	    sendmap( ctx); 
	    dr1Controller_disableLogin( ctx);
	    dr1Controller_enableGame( ctx);
	}
    } else {
        /* invalid login */
	psendMessage(&ctx->ios, DR1MSG_500);
	status = 1;
    }
    return status;
}
	
int
donewplayer( dr1Context *ctx, int argc, char **argv) {
    /* */
    int status = 0;
    int exists;
    exists = ! setLoginPassword( ctx, argv[1], argv[2]);
    if (exists) {
	psendMessage( &ctx->ios, DR1MSG_580, argv[2]);
    } else {
        dr1Playerv_enable( ctx);
	status = dr1Playerv_cmd( ctx, argc, argv);
    }
    return status;
}

int 
handleLogin( dr1Context *ctx, int argc, char **argv) {
    int status = 0;
    if (!strcasecmp( argv[0], "iam")) {
	status = dologin( ctx, argc, argv);
    } else if (!strcasecmp( argv[0], "newplayer")) {
	status = donewplayer( ctx, argc, argv);
    }
    return status;
}

int savegame( dr1Context *ctx) {
    return dr1Context_save( ctx);
}

int domove( dr1Context *ctx, int argc, char **argv) {
    int xpos, ypos;
    dr1MapSquare *gr;

    xpos = ctx->player.location.x;
    ypos = ctx->player.location.y;
    if (!strcasecmp( argv[1], "n")) {
	ypos--;
    } else if (!strcasecmp( argv[1], "s")) {
	ypos++;
    } else if (!strcasecmp( argv[1], "e")) {
	xpos++;
    } else if (!strcasecmp( argv[1], "w")) {
	xpos--;
    } else {
	psendMessage( &ctx->ios, DR1MSG_310, 0, "domove: bad direction");
    }
    gr = &ctx->map->grid[ ypos*ctx->map->xsize + xpos];
    if ( !gr->graphic || 
	 gr->graphic->glyph[0].wall ||
	 ( gr->graphic->glyph[0].door && 
	   !gr->open) 
       )
    {
	if (gr->graphic) gr->seen = 1;
    } else {
	ctx->player.location.x = xpos;
	ctx->player.location.y = ypos;
    }
    sendlocation( ctx);
    return 0;
}

int 
handleGameCmds( dr1Context *ctx, int argc, char **argv) {
    int status=0;
    if (!strcasecmp( argv[0], "move")) {
	status = domove( ctx, argc, argv);
    } else if (!strcasecmp( argv[0], "save")) {
	status = savegame( ctx);
    }
    return status;
}

/*-------------------------------------------------------------------
 * dr1Controller_handleCommand
 *
 *    All commands are processed through this function.  The 'quit' 
 *    command which is always available is handled directly.  All
 *    other commands are passed off to the context handler which
 *    will call the appropriate command handler based on the current
 *    command sets.
 *
 *  PARAMETERS:
 *    ctx   Player context
 *    argc  Number of command args
 *    argv  Array of commands
 *
 *  RETURNS:
 *    0     Processing is OK
 *    1     Fatal processing error.
 *
 *  SIDE EFFECTS:
 */
int dr1Controller_handleCommand( dr1Context *ctx, int argc, char **argv) {

    int status = 0;
    if (argc == 0) {
	psendMessage( &ctx->ios, DR1MSG_101);
    } else {
	if (!strcasecmp( argv[0], "quit")) {
	    status = 1;
	} else {
	    if (dr1Context_handle( ctx, argc, argv)) {
		psendMessage( &ctx->ios, DR1MSG_530, argv[0], ctx->loggedin?"loggedin":"ident");
	    }
	}
    }
    return status;
}

