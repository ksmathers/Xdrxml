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
#include "dr1.h"

/* merchants */
#include "merchant.h"
#include "apothecary.h"
#include "smithy.h"
#include "tanner.h"
#include "wright.h"

/* dialogs */
#include "controller.h"
#include "playerv.h"
#include "class.h"
#include "combatv.h"
#include "town.h"

#define PORT 2300

static dr1Context* ctx[FD_SETSIZE];
enum runstate_t { READABLE, EXECUTE };

/* 
 * init, run, and finit player context -- these three routines are 
 * the main three routines used to wrap the client socket for a player.
 */
void pc_init( int cs) {
    printf("%d: initplayer\n", cs);
    ctx[cs] = calloc( 1, sizeof(dr1Context));
    dr1Stream_create( &ctx[cs]->ios, cs);

    /* announce ourselves on the new player connection */
    psendMessage( &ctx[cs]->ios, DR1MSG_IDENT);
    dr1Controller_enableLogin( ctx[cs]);
}

int pc_runcmd( int cs, enum runstate_t state) {
    /*
     * Tokenizes input from the player connection, and passes it
     * off for to the protocol handler. 
     *
     * A non-zero return indicates that there is a fatal error
     * on the socket, or in the protocol, and that the connection
     * should be closed.
     */
    dr1Context *c = ctx[cs];
    char* cmds[10];
    char  buf[500];
    int i;
    int nargs;
    printf("%d: runplayer (%d)\n", cs, state);

    if (state == READABLE) {
	if ( dr1Stream_fgets( &c->ios, buf, sizeof(buf)) == 0 ) return 1;
	printf("client> %s", buf);

	/* tokenize the command */
	i=0;
	cmds[0] = strtok( buf, " \t\r\n");
	if (cmds[0]) {
	    i++;
	    while ((cmds[i] = strtok( NULL, " \t\r\n")) != 0) {
		if (++i == sizeof(cmds)/sizeof(*cmds)) break;
	    }
	}
	nargs = i;
    } else if (state == EXECUTE) {
	cmds[0] = "tick";
	nargs = 1;
    }

    /* handle it */
    return dr1Controller_handleCommand( c, nargs, cmds);
}

int pc_finit( cs) {
    printf("%d: finitplayer\n", cs);
    dr1Stream_finit( &ctx[cs]->ios);
    if ( ctx[cs]->loggedin) {
        /* did the player complete a login? */
	dr1Context_save( ctx[cs]);
    }
    dr1Context_destroy( ctx[cs]);
    free( ctx[cs]);
    ctx[cs] = NULL;
    return 0;
}

/* main */
int main( int argc, char **argv) {
    int ss, cs, maxsock;
    int xsocks = 0;
    struct sockaddr_in sin;
    struct sockaddr csin;
    fd_set r_set, w_set, e_set, x_set;
    fd_set tr_set, tw_set, te_set;
    int err;
    int reuse;
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
    reuse = 1;
    if (setsockopt( ss, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))) {
        perror("setsockopt"); abort();
    }
    sin.sin_family = AF_INET;
    sin.sin_port = htons( PORT);
    sin.sin_addr.s_addr = INADDR_ANY;
    if (bind( ss, (struct sockaddr *)&sin, sizeof(sin))) { perror("bind"); abort(); }
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
#if 0
	if (err == EINTR) {
	    /* caught a signal */
	    continue;
	}
#endif
	if (err < 0) { perror("select"); abort(); }
	printf("select returned %d\n", err);

        for (i=0; i<=maxsock; i++) {
	    readerr=0;
/*	    printf("checking %d\n", i); /**/

	    if (FD_ISSET( i, &tr_set)) {
	        printf("readable %d\n", i);
		/* readable */
	        if (i == ss) {
		    /* service socket */
		    cs = accept( ss, &csin, &sa_len);
		    printf("%d: Client connect on %d\n", cs, PORT);
		    if (cs < 0) {
			perror("accept");
		    } else {
		        pc_init( cs);
			if (cs > maxsock) maxsock = cs;
			FD_SET( cs, &r_set);
			FD_SET( cs, &e_set);
		    } /* if */
		} else {
		    /* player socket */
		    readerr = pc_runcmd( i, READABLE);
		    if ( dr1Stream_oqlen( &ctx[i]->ios)) {
		        /* data is waiting on the output buffer */
			FD_SET( i, &w_set);
		    }
		}
	    } /* if */

	    if (FD_ISSET( i, &tw_set)) {
		/* writable player socket */
	        int res = dr1Stream_flush( &ctx[i]->ios);
		if (res == 0) { 
		    /* all data has been written from the output buffer */
		    FD_CLR( i, &w_set); 
		}
	    } /* if */

	    if (FD_ISSET( i, &te_set) || readerr) {
		/* errors on a socket */
		printf("%d: Error on socket.\n", i);
		perror("select");
		pc_finit(i);
		close(i);
		FD_CLR( i, &r_set);
		FD_CLR( i, &w_set);
		FD_CLR( i, &e_set);
		FD_CLR( i, &x_set);
	    } /* if */
	    
	    if (FD_ISSET( i, &x_set)) {
	        /* on time slice */
		pc_runcmd( i, EXECUTE);
	    }
	} /* for */
    }
}
