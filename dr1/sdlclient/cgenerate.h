#ifndef __CGENERATE__H
#define __CGENERATE__H
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "player.h"

void
cgenerate_setPlayer( dr1Player *p); 

void 
cgenerate_setErrorStatus( char *msg);

#endif /* __CGENERATE__H */

