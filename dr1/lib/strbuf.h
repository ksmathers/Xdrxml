#ifndef __DR1STRBUF__H
#define __DR1STRBUF__H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif



/*-------------------------------------------------------------------
 * dr1StringBuffer
 *
 *    The structure holds a dynamically expanding string as it is 
 *    being printed to
 */

typedef struct dr1StringBuffer dr1StringBuffer;

struct dr1StringBuffer {
    int bufsize;
    int cpos;
    char *buf;
};


/*-------------------------------------------------------------------
 * sbprintf
 *
 *    The method prints a formatted string to the string buffer. 
 *    Expands the string buffer if needed.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int
sbprintf( dr1StringBuffer *sb, char *fmt, ...); 


/*-------------------------------------------------------------------
 * dr1
 *
 *    The method sbputc adds a character to a string buffer.  It
 *    expands the string buffer if the character would be added 
 *    beyond the end of the array.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int 
sbputc( dr1StringBuffer *sb, char c); 


#endif /* __DR1STRBUF__H */
