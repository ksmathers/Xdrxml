#ifndef __DR1TEXT__H
#define __DR1TEXT__H

#include <SDL.h>
#include <SDL_ttf.h>
#include <assert.h>
#include "player.h"

extern char *race_name[];
extern char *class_name[];


/*-------------------------------------------------------------------
 * dr1Text
 *
 *    The structure holds a text buffer for a single line of screen
 *    text.  The surface is the rendered text, while the char* is 
 *    the text that was rendered.
 */
typedef struct dr1Text dr1Text;
struct dr1Text {
    char text[80];
    SDL_Surface *textbuf;
    SDL_Rect dstrect;
};

/*-------------------------------------------------------------------
 * enum Position
 *
 *    Where to anchor text that is to be rendered.  This value modifies
 *    dstrect.
 */
enum Position {
    ANCHOR_TOPLEFT,
    ANCHOR_CTRLEFT,
    ANCHOR_BOTLEFT,
    ANCHOR_TOPCENTER,
    ANCHOR_CENTER,
    ANCHOR_BOTCENTER,
    ANCHOR_TOPRIGHT,
    ANCHOR_CTRRIGHT,
    ANCHOR_BOTRIGHT
};

/*-------------------------------------------------------------------
 * dr1Text_setInfo
 *
 *    The method modifies the text (and the bitmap) in a dr1Text object
 *
 *  PARAMETERS:
 *    buf     The text to modify
 *    string  What to set the text to
 *    ptsize  Point size of the output text
 *    x,y     X,Y location
 *    pos     Text anchor
 *
 *  RETURNS:
 *    
 *
 *  SIDE EFFECTS:
 */

int dr1Text_setInfo( dr1Text *buf, char *string, int ptsize, int x, int y, enum Position pos) ;

/*-------------------------------------------------------------------
 * dr1Text_infoMessage
 *
 *    The method scrolls the information message display and adds a 
 *    new message to the bottom.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

void
dr1Text_infoMessage( char *s, SDL_Surface *screen);

/*-------------------------------------------------------------------
 * dr1Text_showInfo
 *
 *    The method blits a specific text to the screen.   
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int dr1Text_showInfo( SDL_Surface *screen, dr1Text *txt);

/*-------------------------------------------------------------------
 * dr1Text_init
 *
 *    The method initializes all of the 'normal' text on the screen.
 *    Includes the game title, the character stats, and the scrolling
 *    messages.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int dr1Text_init( SDL_Surface *screen);

/*-------------------------------------------------------------------
 * dr1Text_show
 *
 *    The method shows all of the 'normal' text.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int dr1Text_show( SDL_Surface *screen);

/*-------------------------------------------------------------------
 * dr1Text_setPlayer
 *
 *    The method shows all of the text for a player update
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
int dr1Text_setPlayer( dr1Player *p);
#endif /* __DR1__H */

