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
 * dr1StringBuffer_create
 *
 *    Initialize a new stringbuffer.  
 *
 *  PARAMETERS:
 *    sb   If non-null sb points to a string buffer to be initialized
 *
 *  RETURNS:
 *    The initialized string buffer, newly malloc'd if the 'sb' parameter
 *    is NULL.
 *
 *  SIDE EFFECTS:
 */
dr1StringBuffer*
dr1StringBuffer_create( dr1StringBuffer *sb);

/*-------------------------------------------------------------------
 * sbclear
 *
 *    Clear all data from the buffer, returning it to its just 
 *    initialized state.
 *
 *  PARAMETERS:
 *    sb   StringBuffer
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
void sbclear( dr1StringBuffer *sb);

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
vsbprintf( dr1StringBuffer *sb, char *fmt, va_list va); 

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


/*-------------------------------------------------------------------
 * sbstrcat
 *
 *    Add string to a string buffer
 *
 *  PARAMETERS:
 *     sb   String buffer to modify
 *     str  String to add to the buffer
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int
sbstrcat( dr1StringBuffer *sb, char *str);

/*-------------------------------------------------------------------
 * sbcat
 *
 *    Add string to a string buffer
 *
 *  PARAMETERS:
 *     sb   String buffer to modify
 *     str  String to add to the buffer
 *     len  Length of the string to add
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int
sbcat( dr1StringBuffer *sb, char *str, int len);

/*-------------------------------------------------------------------
 * sbtail
 *
 *    Remove text from a buffer, leaving only the tail
 *
 *  PARAMETERS:
 *     sb          String buffer to modify
 *     first_char  First character of the revised buffer
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

int
sbtail( dr1StringBuffer *sb, int first_char);

/*-------------------------------------------------------------------
 * sbindex
 *
 *    Find a character in the buffer
 *
 *  PARAMETERS:
 *     sb   String buffer to modify
 *     ch   Character to look for
 *
 *  RETURNS:
 *     Index of the character matching 'ch', or -1 if not found.
 *
 *  SIDE EFFECTS:
 */

int
sbtail( dr1StringBuffer *sb, int ch);
#endif /* __DR1STRBUF__H */
