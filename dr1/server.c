/* inet sockets */
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>

/* xml library */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpointer.h>

/* game includes */
#include "globals.h"
#include "player.h"
#include "context.h"
#include "qio.h"
#include "lib/xdrxml.h"
#include "lib/protocol.h"

/* merchants */
#include "apothecary.h"
#include "smithy.h"
#include "tanner.h"
#include "wright.h"

/* dialogs */
#include "playerv.h"
#include "class.h"
#include "combatv.h"
#include "town.h"

#define PORT 2300
enum states {
    INIT,
    LOGIN,
    ROLLPLAYERDIALOG,
    READY
};

enum runstate {
    EXECUTE, 
    READABLE
};

static dr1Context* ctx[FD_SETSIZE];

/*
 * sendmap 
 *
 */
void sendmap( dr1Context* ctx) {
    char *buf;
    xdrxmlsb_reset( &xdrxmlsb);
    if (xdr_dr1Map( &xdrxmlsb, ctx->map) != TRUE) {
	printf("Error serializing map.\n");
    }

    buf = xdrxmlsb_getbuf( &xdrxmlsb); 

    qprintf( ctx, "%s %d %d %d %d\n", DR1MSG_120, 0, 0, ctx->map->xsize, ctx->map->ysize);
    qprintf( ctx, buf);
    qprintf( ctx, "%s\n", SEPARATOR);
}

/*
 * sendplayer 
 *
 */
void sendplayer( dr1Context* ctx) {
    char *buf;
    xdrxmlsb_reset( &xdrxmlsb);
    if (xdr_dr1Player( &xdrxmlsb, &ctx->player) != TRUE) {
	printf("Error serializing player.\n");
    }

    buf = xdrxmlsb_getbuf( &xdrxmlsb); 

    qprintf( ctx, "%s\n", DR1MSG_170);
    qprintf( ctx, buf);
    qprintf( ctx, "%s\n", SEPARATOR);
}

/*--------------------------------------------------------------------------
 * loginplayer
 *
 *     Runs through the login sequence.  Calls dr1Playerv_showDialog to 
 *     create a new login if needed.
 *
 *     This is one of several input handling routines that returns 
 *     immediately each time it is called.  If the function is blocked
 *     on input it should set errno to EWOULDBLOCK and return 1.
 *
 *     Normal return is zero, indicating that the function at the top
 *     of the call stack should be called the next time there is 
 *     an execution slice, or there is data ready for input.
 *
 *   Parameters:
 *     ctx    Player context
 *
 *   Returns:
 *     0      Continue processing
 *     1      Context exception 
 *     errno is set to various error conditions
 *          EWOULDBLOCK if no input is waiting and the socket would be read
 *          ENOSYS internal error, system is in an invalid state
 *
 *   Side Effects:
 *     ctx is changed to reflect the state of the interaction with the user
 *     data is read or written to the player socket
 */
int loginplayer( dr1Context *ctx) {
    struct stat pdat;
    int pdatf;
    int loadOk;
    char *cpos;
    char *tok;
    char buf[80];
    struct {
        int state;
	char *login;
	char *password;
    } *autos = dr1Context_auto( ctx, sizeof(*autos));
    enum { INIT, LOGIN, PASSWORD, NEWPLAYER };

    printf("%d: loginplayer\n", fileno(ctx->fp));

    switch (autos->state) {
	case INIT:
	    qprintf( ctx, "DR1/1.0\n");
	    autos->state = LOGIN;
	    return 0;

        case LOGIN:
	    if (!ctx->inputready) { errno=EWOULDBLOCK; return 1; }
	    qgets( buf, sizeof(buf), ctx);

	    tok = strtok( buf, " ");
	    if (!strcmp( tok, "IAM")) {
	        /* login request */
		autos->login = strdup( strtok( NULL, " "));
		autos->password = strdup( strtok( NULL, " "));
		autos->state = PASSWORD;
		return 0;
	    }
	    qprintf( ctx, DR1MSG_510);
	    return 0;

	case PASSWORD:
	    for (cpos = autos->login; *cpos != 0; cpos++) {
		if (!isalnum(*cpos)) *cpos = '_';
	    }

	    for (cpos = autos->password; *cpos != 0; cpos++) {
		if (!isalnum(*cpos)) *cpos = '_';
	    }

	    strncpy( ctx->fname, autos->login, sizeof(ctx->fname)-4);
	    strncat( ctx->fname, "-", sizeof(ctx->fname)-4);
	    strncat( ctx->fname, autos->password, sizeof(ctx->fname)-4);
	    strcat( ctx->fname, ".xml");
	    ctx->fname[ sizeof(ctx->fname)-1] = 0;

	    pdatf = stat( ctx->fname, &pdat);
	    if (!pdatf && S_ISREG(pdat.st_mode)) {
		loadOk = dr1Context_load( ctx, ctx->fname);
		if (!loadOk) {
		    qprintf(ctx, DR1MSG_520, ctx->fname);
		    return 1;
		}
		qprintf(ctx, DR1MSG_100);
		sendplayer( ctx);
		sendmap( ctx);
		dr1Context_popcall( ctx, 0);
	    } else {
	        autos->state = NEWPLAYER;
	        dr1Context_pushcall( ctx, dr1Playerv_showDialog);
	    }
	    return 0;
	
	case NEWPLAYER:
	    qprintf(ctx, DR1MSG_100);
	    ctx->map = dr1Map_readmap( "lib/maps/town.map");
	    ctx->player.location.x = ctx->map->startx;
	    ctx->player.location.y = ctx->map->starty;
	    sendplayer( ctx);
	    sendmap( ctx);
	    dr1Context_popcall( ctx, ctx->cstack[ctx->stackptr-1].result);
	    return 0;

	default:
	    qprintf(ctx, DR1MSG_570, "Invalid state");
	    printf( "%d: Invalid state.\n", fileno(ctx->fp));
	    errno = EINVAL;
	    return 1;
    }
    return 0;
}

int savegame( dr1Context *ctx) {
    return dr1Context_save( ctx);
}

int playermain( dr1Context *ctx) {
    int *state;
    enum { LOGIN, TOWN, OUTOFTOWN, DUNGEON, OUTOFDUNGEON, DEAD, EXITING };
    
    printf("%d playermain\n", fileno(ctx->fp));
    state = dr1Context_auto( ctx, sizeof(*state));

    switch (*state) {
	case LOGIN:
	    dr1Context_pushcall( ctx, loginplayer);
	    *state = TOWN;
	    return 0;

	case TOWN:
	    if (HITPOINTS(&ctx->player) < -10) {
		*state = DEAD;
		return 0;
	    }
	    dr1Context_pushcall( ctx, dr1Town_showDialog);
	    *state = OUTOFTOWN;
	    return 0;

	case OUTOFTOWN:
	    savegame( ctx);
	    if (dr1Context_return( ctx) == 'x') *state = EXITING;
	    else *state = DUNGEON;
	    return 0;

	case DUNGEON:
	    dr1Context_pushcall( ctx, dr1Combatv_showDialog);
	    *state = OUTOFDUNGEON;
	    return 0;

	case OUTOFDUNGEON:
	    savegame( ctx);
	    if (dr1Context_return( ctx) == 'd') *state = DEAD;
	    else *state = TOWN;
	    return 0;

	case DEAD:
	    qprintf( ctx, "O Mater Dei, memento mei.\n");
	    qprintf( ctx, "You have not been resurrected (yet).\n");
	    *state = EXITING;

	case EXITING:
	    savegame( ctx);
	    errno = ENOSYS;
	    return 1;
    }
    return 0;
}


/* 
 * init, run, and finit player -- these three routines are the main
 * three routines used to wrap the client socket for a player.
 */
void initplayer( int cs) {
    printf("%d: initplayer\n", cs);
    ctx[cs] = calloc( 1, sizeof(dr1Context));
    ctx[cs]->fp = fdopen( cs, "r+");
    dr1Context_pushcall( ctx[cs], playermain);
}

int runplayer( int cs, enum runstate st) {
    dr1Context *c = ctx[cs];
    printf("%d: runplayer\n", cs);
    if (st == READABLE) c->inputready = 1;
    else c->inputready = 0;

    /* call the command at the top of the command stack */
    if (c->stackptr <= 0) return -1;
    return c->cstack[ c->stackptr - 1].cmd( c);
}

int finitplayer( cs) {
    printf("%d: finitplayer\n", cs);
    fclose( ctx[cs]->fp);
    free( ctx[cs]);
    ctx[cs] = NULL;
    return 0;
}

int main( int argc, char **argv) {
    int ss, cs, maxsock;
    int xsocks;
    struct sockaddr_in sin;
    struct sockaddr csin;
    fd_set r_set, w_set, e_set, x_set;
    fd_set tr_set, tw_set, te_set;
    int err;
    struct timeval ttv;
    struct timeval tv_slow = { /* tv_sec */ 10, /* tv_usec */ 500000 };
    struct timeval tv_fast = { /* tv_sec */ 0,  /* tv_usec */ 0 };

    /* initialize XML library */
    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);
    
    /* initialize globals */
    dr1Dice_seed();
    dr1Apothecary_init( &dr1apothecary);
    dr1Tanner_init();
    dr1Wright_init();
    dr1Smithy_init();
    dr1GlyphSet_init();

    /* open server socket */
    ss = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sin.sin_family = AF_INET;
    sin.sin_port = htons( PORT);
    sin.sin_addr.s_addr = INADDR_ANY;
    if (bind( ss, &sin, sizeof(sin))) { perror("bind"); abort(); }
    if (listen( ss, 10)) { perror("listen"); abort(); }

    /* setup for select */
    maxsock = ss;
    FD_ZERO( &r_set);		/* socket is readable (READABLE) */
    FD_ZERO( &w_set);		/* write queued data */
    FD_ZERO( &e_set);		/* error on socket */
    FD_ZERO( &x_set);		/* socket timeslicing (EXECUTE) */
    FD_SET( ss, &r_set);
    FD_SET( ss, &e_set);

    printf("%d: Server ready...\n", ss);
    for (;;) {
        int i;
	int readerr;
	int sa_len;
        tr_set = r_set; tw_set = w_set; te_set = e_set;

        /*
	 * choose the select timeout.  If time slicing, use a fast
	 * select timeout.  If not time slicing save CPU.
	 */
	if (xsocks) {
	    ttv = tv_fast;
	} else {
	    ttv = tv_slow;
	}
        
        err = select( maxsock + 1, &tr_set, &tw_set, &te_set, &ttv);
	if (err == EINTR) continue;
	if (err < 0) { perror("select"); abort(); }
	printf("select returned %d\n", err);

        xsocks=0;
        for (i=0; i<=maxsock; i++) {
	    readerr=0;
/*	    printf("checking %d\n", i); /**/

	    if (FD_ISSET( i, &tr_set)) {
	        printf("readable %d\n", i);
		/* readable */
	        if (i == ss) {
		    cs = accept( ss, &csin, &sa_len);
		    printf("%d: Client connect on %d\n", cs, PORT);
		    if (cs < 0) {
			perror("accept");
		    } else {
		        initplayer( cs);
			if (cs > maxsock) maxsock = cs;
			FD_SET( cs, &r_set);
			FD_SET( cs, &e_set);
			FD_SET( cs, &x_set);
			xsocks++;		/* turn on timeslicing */
		    } /* if */
		} else {
		    FD_SET( i, &x_set); /* mark executable */
		    readerr = runplayer( i, READABLE);
		}
	    } /* if */

	    if (FD_ISSET( i, &tw_set)) {
		/* writable */
		/*
		 * FIXME: right now qprintf is synchronous, so
		 * this doesn't do anything yet.  Eventually
		 * this should flush queues as write space becomes
		 * available on the socket
		 */
	    } /* if */

            if (FD_ISSET( i, &x_set)) {
	        xsocks++;
		readerr = runplayer(i, EXECUTE); 
		if (readerr) {
		    if (errno == EWOULDBLOCK) {
		        readerr = 0;
		        xsocks--;
			FD_CLR( i, &x_set);
		    } 
		}
	    }

	    if (FD_ISSET( i, &te_set) || readerr) {
		/* errors */
		printf("%d: Error on socket.\n", i);
		perror("select");
		finitplayer(i);
		close(i);
		FD_CLR( i, &r_set);
		FD_CLR( i, &w_set);
		FD_CLR( i, &e_set);
		FD_CLR( i, &x_set);
	    } /* if */
	} /* for */
    }
}
