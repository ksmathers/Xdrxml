#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <rpc/xdr.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "stream.h"
#include "protocol.h"
#include "xdrxml.h"
#include "../player.h"

static struct {
    int sd;
    dr1Stream ios;
} ctx = { 0 };

int doConnect( char *server, int port) {
    struct sockaddr_in addr;
    struct hostent *svr_addr;
    svr_addr = gethostbyname( server);
    assert(svr_addr);
    addr.sin_family = AF_INET;
    addr.sin_addr = *(struct in_addr *)svr_addr->h_addr;
    addr.sin_port = htons( port);
    ctx.sd = socket( PF_INET, SOCK_STREAM, 0);

    if (connect( ctx.sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) {
	perror("connect");
	exit(1);
    }
    dr1Stream_create( &ctx.ios, ctx.sd);
    return 0;
}

int handleMessage(char *buf) {
    static int mode = 0;
    static dr1StringBuffer *sb = NULL;
    if (!sb) {
	sb = dr1StringBuffer_create( NULL);
    }
    switch (mode) {
	case 0:
	    if (!strncmp( buf, MAPDATA, 3)) {
		sb->cpos = 0;
		mode = 1;
	    }
	    if (!strncmp( buf, PLAYERDATA, 3)) {
		sb->cpos = 0;
		mode = 2;
	    }
	    break;
	case 1: /* reading map data */
	case 2: /* reading player data */
	    if ( !strcmp( buf, SEPARATOR)) {
		XDR xdrs;
		int ok;
		xdr_xml_sb_create( &xdrs, sb->buf, XDR_DECODE);
		switch (mode) {
		    case 1: /* reading map data */
			printf("Got map.\n");
			break;
		    case 2: /* reading player data */
			printf("Got player data \n");
			{
			    dr1Player p;
			    ok = xdr_dr1Player( &xdrs, &p);
			    if (!ok) {
				printf("Error decoding player data\n");
			    }
			}
			break;
		}
		break;
	    }
	    sbstrcat( sb, buf);
	    break;
    } /* switch */
    return 0;
}

void* comm_main( void* iparm) {
    fd_set r_set, w_set, e_set;
    fd_set tr_set, tw_set, te_set;
    int maxsock;
    int err;
    char buf[80];
    struct timeval ttv;
    struct timeval short_wait = { 0, 100000 };
    char *server = (char *)iparm;


    FD_ZERO( &r_set);		/* socket is readable (READABLE) */
    FD_ZERO( &w_set);		/* write queued data */
    FD_ZERO( &e_set);		/* error on socket */

/*    dr1Text_infoMessage( "Connecting to server...", ctx.screen); */
    doConnect( server, 2300);
/*    dr1Text_infoMessage( "Connected.", ctx.screen); */
    FD_SET( ctx.sd, &r_set);
    maxsock = ctx.sd;
    for(;;) {
	tr_set = r_set; tw_set = w_set; te_set = e_set;
	ttv = short_wait;
        
        err = select( maxsock + 1, &tr_set, &tw_set, &te_set, &ttv);
	printf("/"); fflush(stdout);
	if (err == EINTR) continue;
	if (err < 0) { perror("select"); abort(); }
	if (FD_ISSET( ctx.sd, &tr_set)) {
	    printf("R\n");

	    /* data from server ready to read */
	    if ( dr1Stream_gets( &ctx.ios, buf, sizeof(buf)) == 0) {
		perror("read"); continue; 
	    }
	    printf("buf %s\n",buf); /**/
/*	    dr1Text_infoMessage( buf, ctx.screen); */
            handleMessage( buf);
	} /* if */
    }
}
