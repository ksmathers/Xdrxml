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

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "stream.h"
#include "protocol.h"
#include "xdrxml.h"
#include "lib/map.h"
#include "player.h"
#include "common.h"

GtkDestroyNotify onSocketDestroy;

enum {
    M_IDENT,
    M_LOGIN,
    M_READY,
    M_CCMAPDATA,
    M_CCPLAYERDATA,
    M_MAPDATA,
    M_PLAYERDATA,
    M_LOCATIONDATA,
    M_CHARACTERCREATEDIALOG,
    M_INVENTORYDIALOG
};

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
                if (common.login) {
		    psendMessage( &common.ios, DR1CMD_LOGIN, common.name, common.password);
		    mode = M_LOGIN;
		} else {
		    psendMessage( &common.ios, DR1CMD_NEWPLAYER, common.name, common.password);
		    mode = M_CHARACTERCREATEDIALOG;
		}
	    } else {
		printf("PROTOCOL ERROR: Expecting '%s', got '%s'\n", DR1MSG_IDENT, buf);
	    }
	    break;

        case M_CHARACTERCREATEDIALOG:
	    if (!strncmp( buf, DR1MSG_105, 3)) {
		GtkWidget *wgenerate = glade_xml_get_widget( common.glade, "wgenerate");
		common.dialog = WGENERATE;
	        gtk_widget_show( wgenerate);
	    } else if (!strncmp( buf, DR1MSG_120, 3)) {
	        mode = M_CCMAPDATA;
	    } else if (!strncmp( buf, DR1MSG_170, 3)) {
	        mode = M_CCPLAYERDATA;
	    } else if (!strncmp( buf, DR1MSG_100, 3)) {
		GtkWidget *wgenerate = glade_xml_get_widget( common.glade, "wgenerate");
	        gtk_widget_hide( wgenerate);
		common.dialog = NONE;
	        mode = M_READY;
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
	case M_CCMAPDATA: 	/* reading map data */
	case M_PLAYERDATA: 	/* reading player data */
	case M_CCPLAYERDATA: 	/* reading player data */
	    if ( !strcmp( buf, SEPARATOR)) {
		XDR xdrs;
		int ok;
		xdr_xml_sb_create( &xdrs, sb->buf, XDR_DECODE);
		switch (mode) {
		    case M_MAPDATA:    /* reading map data */
		    case M_CCMAPDATA:    /* reading map data */
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
		    case M_CCPLAYERDATA: /* reading player data */
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
		if (mode == M_CCMAPDATA || mode == M_CCPLAYERDATA) {
		    mode = M_CHARACTERCREATEDIALOG;
		} else {
		    mode = M_READY;
		}
		break;
	    }
	    sbstrcat( sb, buf);
	    break;
    } /* switch */
    return 0;
}

void onInputReady( gpointer userdata, gint socket, GdkInputCondition cond) {
    assert(socket == common.sd);

    if (cond & GDK_INPUT_EXCEPTION) {
        perror("readexception");
    }

    while (cond & GDK_INPUT_READ || dr1Stream_iqlen( &common.ios)) {
	/* data from server ready to read */
	char buf[1024];

	printf("R\n");
        cond &= !GDK_INPUT_READ;

	if ( dr1Stream_fgets( &common.ios, buf, sizeof(buf)) == 0) {
	    /* no complete lines ready */
	    perror("read"); 
	    break; 
	}
/*	printf("buf '%s'\n",buf); /**/
/*	dr1Text_infoMessage( buf, common.screen); /**/
	handleMessage( buf);
    } /* if */
}

void onOutputReady( gpointer userdata, gint socket, GdkInputCondition cond) {
    assert( socket == common.sd);
    dr1Stream_flush( &common.ios);
    if (dr1Stream_oqlen( &common.ios) == 0) {
        /* no more output queued.  remove output handler */
	gtk_input_remove( common.write_uid);
    }
}

int doConnect( void) {
    char *server = common.server;
    int port = 2300;
    struct sockaddr_in addr;
    struct hostent *svr_addr;
    long flags;

    svr_addr = gethostbyname( server);
    assert(svr_addr);
    addr.sin_family = AF_INET;
    addr.sin_addr = *(struct in_addr *)svr_addr->h_addr;
    addr.sin_port = htons( port);
    common.sd = socket( PF_INET, SOCK_STREAM, 0);

    if (connect( common.sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) 
    {
	perror("connect");
	exit(1);
    }
    flags = fcntl( common.sd, F_GETFL);
    if (flags < 0) {
	perror("fcntl F_GETFL");
	exit(1);
    }
    flags |= O_NONBLOCK;
    if ( fcntl( common.sd, F_SETFL, flags)) {
	perror("fcntl F_SETFL");
	exit(1);
    }

    dr1Stream_create( &common.ios, common.sd);

    common.read_uid = gtk_input_add_full( 
            common.sd, GDK_INPUT_READ | GDK_INPUT_EXCEPTION, 
	    onInputReady, NULL, NULL, onSocketDestroy);
    return 0;
}



int sendCommand(char key) {
    switch (key) {
        case 'n':
        case 'e':
        case 's':
        case 'w':
	    psendMessage( &common.ios, DR1CMD_MOVE, key);
    } /* switch key */

    if (dr1Stream_oqlen( &common.ios)>0) {
        /* Queued output */
        common.write_uid = gtk_input_add_full( 
            common.sd, GDK_INPUT_WRITE,
	    onOutputReady, NULL, NULL, NULL);
    }
    return 0;
}


