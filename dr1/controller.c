#include <ctype.h>
#include <sys/stat.h>

#include "controller.h"
#include "playerv.h"
#include "town.h"

#include "lib/protocol.h"
#include "lib/xdrxml.h"

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

    dr1Stream_printf( &ctx->ios, DR1MSG_120, 
	    0, 0, ctx->map->xsize, ctx->map->ysize);
    res = dr1Stream_printf( &ctx->ios, buf);
    if (res != strlen(buf)) perror("qprintf-1");
    res = dr1Stream_printf( &ctx->ios, "%s\n", SEPARATOR);
    if (res != strlen(SEPARATOR)) perror("qprintf-2");
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

    dr1Stream_printf( &ctx->ios, "%s\n", DR1MSG_170);
    res = dr1Stream_printf( &ctx->ios, buf);
    if (res != strlen(buf)) perror("qprintf-3");
    res = dr1Stream_printf( &ctx->ios, "%s\n", SEPARATOR);
    if (res != strlen(SEPARATOR)) perror("qprintf-4");
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
int dr1Controller_handleLogin( dr1Context *ctx, int argc, char **argv) {
    int loadOk;
    int exists;

    printf("%d: handleLogin\n", ctx->ios.fd);

    exists = setLoginPassword( ctx, argv[1], argv[2]);

    if (exists) {
        /* player data file is found */
	loadOk = dr1Context_load( ctx, ctx->fname);
	if (!loadOk) {
	    dr1Stream_printf(&ctx->ios, DR1MSG_520, ctx->fname);
	    return 1;
	}
	dr1Stream_printf(&ctx->ios, DR1MSG_100);
	sendplayer( ctx);
	sendmap( ctx); 
	return 0;
    } else {
        /* invalid login */
	dr1Stream_printf(&ctx->ios, DR1MSG_500);
	return 0;
    }
    return 0;
}
	
int
dr1Controller_handleNewPlayer( dr1Context *ctx, int argc, char **argv) {
    /* */
    int status = 0;
    int exists;
    if (!strcasecmp( argv[0], "cancel")) {
	ctx->dialog = CANCEL;
    } else if (!strcasecmp( argv[0], "newplayer")) {
	exists = setLoginPassword( ctx, argv[1], argv[2]);
	if (exists) {
	    dr1Stream_printf( &ctx->ios, DR1MSG_580, argv[2]);
	} else {
	    dr1Stream_printf( &ctx->ios, DR1MSG_200);
	    status = dr1Playerv_cmd( ctx, argc, argv);
	}
    } else {
        status = dr1Playerv_cmd( ctx, argc, argv);
	if (ctx->dialog == DIALOG_DONE) {
	    dr1Stream_printf(&ctx->ios, DR1MSG_100);
	    ctx->map = dr1Map_readmap( "lib/maps/town.map");
	    ctx->player.location.x = ctx->map->startx;
	    ctx->player.location.y = ctx->map->starty;
	    sendplayer( ctx);
	    sendmap( ctx);
	}
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
	dr1Stream_printf( &ctx->ios, DR1MSG_310, 0, "domove: bad direction");
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
    return 0;
}

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
int dr1Controller_handleCommand( dr1Context *ctx, int argc, char **argv) {
    int status = 0;
    if (!strcasecmp( argv[0], "quit")) {
	status = 1;
    } else if (ctx->state == LOGIN) {
	if (!strcasecmp( argv[0], "iam")) {
	    status = dr1Controller_handleLogin( ctx, argc, argv);
	    if (!status) {
	        /* loaded ok */
		if (ctx->map->town) {
		    ctx->state = TOWN;
		} else {
		    ctx->state = DUNGEON;
		}
	    }
	} else if (!strcasecmp( argv[0], "newplayer")) {
	    int exists;
	    exists = setLoginPassword( ctx, argv[1], argv[2]);
	    if (exists) {
		dr1Stream_printf( &ctx->ios, DR1MSG_580, argv[2]);
	    } else {
		status = dr1Controller_handleNewPlayer( ctx, argc, argv);
		dr1Stream_printf( &ctx->ios, DR1MSG_200);
		ctx->dialog = ACTIVE;
		ctx->state = NEWPLAYER;
	    }
	} else {
	    dr1Stream_printf( &ctx->ios, DR1MSG_530, argv[0], "LOGIN");
	}
    } else if (ctx->state == NEWPLAYER) {
	status = dr1Controller_handleNewPlayer( ctx, argc, argv);
	if (ctx->dialog == DIALOG_DONE) {
	    /* loaded ok */
	    if (ctx->map->town) {
		ctx->state = TOWN;
	    } else {
		ctx->state = DUNGEON;
	    }
	} else if (ctx->dialog == CANCEL) {
	    ctx->state = LOGIN;
	}
    } else {
	/* In game */

	/* global commands (dead or alive) */
	if (!strcasecmp( argv[0], "talk")) {
/*	    status = dotalk( ctx, argc, argv); */
	} else if (ctx->player.hp < -10) {
	    /* player is dead */
/*	    dr1Dead_cmd( ctx, argc, argv); */
	    status = 0;
	} else {
	    /* player is alive */
	    /* global commands */
	    if (!strcasecmp( argv[0], "move")) {
		status = domove( ctx, argc, argv);
	    } else if (ctx->state == TOWN) {
		/* player in town map */
/*		dr1Town_cmd( ctx, argc, argv); */
	    } else if (ctx->state == DUNGEON) {
	        /* player in dungeon map */
/*		dr1Dungeon_cmd( ctx, argc, argv); */
	    } else {
		/* not a valid state */
		dr1Stream_printf( &ctx->ios, DR1MSG_540);
		status = -1;
	    }
	}
    }
    return status;
}

