#ifndef __DR1COMMON__H
#define __DR1COMMON__H

#include <SDL.h>
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
