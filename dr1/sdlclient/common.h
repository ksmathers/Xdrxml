#ifndef __DR1COMMON__H
#define __DR1COMMON__H

#include <SDL.h>
#include <glade/glade.h>
#include "player.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */

typedef struct {
    dr1Player player;
#if 0
    dr1Map map;
#endif
    char *server;
    char *name;
    char *password;
    SDL_Surface *screen;
    dr1Map* map;
    int xpos, ypos;
    int login;		/* true for login, false for new player */
    enum { NONE, WGENERATE } dialog;
    GladeXML *glade;

    /* communications stream */
    int sd;
    dr1Stream ios;
    int read_uid;
    int write_uid;
} Common;

extern Common common;

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */



#endif /* __DR1COMMON__H */
