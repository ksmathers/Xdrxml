#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <rpc/xdr.h>
#include <fcntl.h>

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "stream.h"
#include "protocol.h"
#include "xdrxml.h"
#include "lib/map.h"
#include "player.h"
#include "common.h"

static struct {
    int sd;
    dr1Stream ios;
    Common *common;
} ctx = { 0 };

int doConnect( char *server, int port) {
    struct sockaddr_in addr;
    struct hostent *svr_addr;
    long flags;

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
    flags = fcntl( ctx.sd, F_GETFL);
    if (flags < 0) {
	perror("fcntl F_GETFL");
	exit(1);
    }
    flags |= O_NONBLOCK;
    if ( fcntl( ctx.sd, F_SETFL, flags)) {
	perror("fcntl F_SETFL");
	exit(1);
    }

    dr1Stream_create( &ctx.ios, ctx.sd);
    return 0;
}

enum {
    M_IDENT,
    M_LOGIN,
    M_READY,
    M_MAPDATA,
    M_PLAYERDATA,
    M_LOCATIONDATA,
    M_CHARACTERCREATEDIALOG,
    M_INVENTORYDIALOG
};

int sendCommand(char key) {
    switch (key) {
        case 'n':
        case 'e':
        case 's':
        case 'w':
	    dr1Stream_printf( &ctx.ios, DR1CMD_MOVE, key);
    } /* switch key */
    return 0;
}

int handleMessage(char *buf) {
    static int mode = M_IDENT;
    static dr1StringBuffer *sb = NULL;
    if (!sb) {
	sb = dr1StringBuffer_create( NULL);
    }
    switch (mode) {
	case M_IDENT:		/* waiting for IDENT */
	    if (!strcmp( buf, DR1MSG_IDENT)) {
/*		showDialog( DLG_LOGIN); */
 		psendMessage( &ctx.ios, DR1CMD_LOGIN, common.name, common.password);
		mode = M_LOGIN;
	    } else {
		printf("Expecting '%s', got '%s'\n", DR1MSG_IDENT, buf);
	    }
	    break;

	case M_LOGIN:
	    if (!strncmp( buf, DR1MSG_100, 3)) {
/*		closeDialog( DLG_LOGIN); */
		mode = M_READY;
	    } else if (!strncmp( buf, DR1MSG_105, 3)) {
/*		openDialog( DLG_CHARACTERCREATE); */
	        mode = M_CHARACTERCREATEDIALOG;
	    } else if (!strncmp( buf, DR1MSG_500, 3)) {
		printf("Name/password error\n");
#if 0
	        resetDialog( DLG_LOGIN);
		showStatus( "Name/password error"); 
#endif
	    } else if (!strncmp( buf, DR1MSG_550, 3)) {
		printf("Connection denied. Blacklisted.\n");
#if 0
		showStatus( "Connection denied.  You are blacklisted.");
#endif
		mode = M_IDENT;
	    } else {
/*		showStatus( "Server Error."); */
		mode = M_IDENT;
	    }
	    break;

	case M_READY:	/* ready for next message */
	    if (!strncmp( buf, DR1MSG_120, 3)) {
		mode = M_MAPDATA;
	    } else if (!strncmp( buf, DR1MSG_170, 3)) {
		mode = M_PLAYERDATA;
	    } else if (!strncmp( buf, DR1MSG_175, 3)) {
                dr1Location l;
                int n;
                n = precvMessage( buf, DR1MSG_175, &l.mapname, &l.x, &l.y);
                if (n != 3) {
                    printf("Unable to parse '%s'\n", buf);
		} else {
		    setlocation( &l);
		}
	    } 
#if 0
	    else if (!strncmp( bud, DR1MSG_195, 3)) {
/*		openDialog( DLG_INVENTORYDIALOG); */
	        mode = M_INVENTORYDIALOG;
	    }
#endif
	    break;

	case M_MAPDATA: 	/* reading map data */
	case M_PLAYERDATA: 	/* reading player data */
	    if ( !strcmp( buf, SEPARATOR)) {
		XDR xdrs;
		int ok;
		xdr_xml_sb_create( &xdrs, sb->buf, XDR_DECODE);
		switch (mode) {
		    case M_MAPDATA:    /* reading map data */
			printf("Got map.\n");
			{
			    dr1Map* map;
			    map = calloc( 1, sizeof( dr1Map));
			    ok = xdr_dr1Map( &xdrs, map);
			    if (!ok) {
				printf("Error decoding map data\n");
				break;
			    }
			    setmap( map);
			}
			break;
		    case M_PLAYERDATA: /* reading player data */
			printf("Got player data \n");
			{
			    dr1Player *p;
			    p = calloc( 1, sizeof(dr1Player));
			    ok = xdr_dr1Player( &xdrs, p);
			    if (!ok) {
				printf("Error decoding player data\n");
				break;
			    }
			    setplayer( p);
			}
			break;
		}
		sbclear( sb);
		mode = M_READY;
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
    int more=0;
    char buf[80];
    struct timeval ttv;
    struct timeval short_wait = { 0, 100000 };
    struct timeval no_wait = { 0, 0 };
    Common *common = iparm;
    ctx.common = common;


    FD_ZERO( &r_set);		/* socket is readable (READABLE) */
    FD_ZERO( &w_set);		/* write queued data */
    FD_ZERO( &e_set);		/* error on socket */

/*    dr1Text_infoMessage( "Connecting to server...", common->server); */
    doConnect( common->server, 2300);
/*    dr1Text_infoMessage( "Connected.", ctx.screen); */
    FD_SET( ctx.sd, &r_set);
    maxsock = ctx.sd;
    for(;;) {
	tr_set = r_set; tw_set = w_set; te_set = e_set;
	ttv = short_wait;
        
        if (more && dr1Stream_iqlen( &ctx.ios)) {
            ttv = no_wait;
        }

	err = select( maxsock + 1, &tr_set, &tw_set, &te_set, &ttv);
/*	printf("/"); fflush(stdout); /**/
	if (err == EINTR) continue;
	if (err < 0) { perror("select"); abort(); }
	if (FD_ISSET( ctx.sd, &tr_set) || dr1Stream_iqlen( &ctx.ios)) {
/*	    printf("R\n"); */

	    /* data from server ready to read */
	    if ( dr1Stream_fgets( &ctx.ios, buf, sizeof(buf)) == 0) {
                more=0;
		perror("read"); continue; 
	    }
            more=1;
/*	    printf("buf '%s'\n",buf); /**/
/*	    dr1Text_infoMessage( buf, ctx.screen); */
            handleMessage( buf);
	} /* if */
    }
}
