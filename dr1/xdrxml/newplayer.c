#include <rpc/xdr.h>
#include "xdrxml.h"

struct Location {
    int x;
    int y;
};

struct Player {
    char *name;
    struct Location loc;
    int health;
};

bool_t xdr_Location( XDR *xdrs, struct Location *l) {
    xdr_attr( xdrs, "x");
    if (!xdr_int( xdrs, &l->x)) return FALSE;

    xdr_attr( xdrs, "y");
    if (!xdr_int( xdrs, &l->y)) return FALSE;
    return TRUE;
}

bool_t xdr_Player( XDR *xdrs, struct Player *p) {
    xdr_attr( xdrs, "name");
    if (!xdrxml_wrapstring( xdrs, &p->name)) return FALSE;

    xdr_push_note( xdrs, "loc");
    if (!xdr_Location( xdrs, &p->loc)) return FALSE;
    xdr_pop_note( xdrs);

    xdr_attr( xdrs, "health");
    if (!xdr_int( xdrs, &p->health)) return FALSE;

    return TRUE;
}


int main( int argc, char **argv) {
    XDR xdrs; 
    struct Player bob = {"William", {50, 84}, 100};
    int op;

    if (argc != 2) {
	printf("Usage: newplayer (read|write)\n");
	exit(1);
    }

    if (!strcmp( argv[1], "read")) {
	op = XDR_DECODE;
    } else {
	op = XDR_ENCODE;
    }

    xdr_xml_create( &xdrs, "player.xml", op);
    xdr_push_note( &xdrs, "bob");
    xdr_Player( &xdrs, &bob);
    xdr_pop_note( &xdrs);
    xdr_destroy( &xdrs); 	

    if (op == XDR_DECODE) {
	printf("  Player: %s\n", bob.name);
	printf("Location: (%d,%d)\n", bob.loc.x, bob.loc.y);
	printf("  Health: %d%%\n", bob.health);
    }
    return 0;
}


