#ifndef __DR1STREAM__H
#define __DR1STREAM__H
#include "strbuf.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure dr1Stream adds stream semantics to a socket
 */
typedef struct dr1Stream {
    int fd;
    int inputready;
    int error;
    dr1StringBuffer ibuf;
    dr1StringBuffer obuf;
} dr1Stream;


/*-------------------------------------------------------------------
 * dr1Stream_create
 *
 *    The method constructs a Stream
 *
 *  PARAMETERS:
 *    str   Stream to intialize, or NULL to calloc the stream
 *    sd    Socket to connect the stream to
 *
 *  RETURNS:
 *    The initialized stream
 *
 *  SIDE EFFECTS:
 */
dr1Stream* 
dr1Stream_create( dr1Stream *str, int sd);

/*-------------------------------------------------------------------
 * dr1Stream_finit
 *
 *    The method destroys a stream
 *
 *  PARAMETERS:
 *    str   Stream to intialize, or NULL to calloc the stream
 *
 *  SIDE EFFECTS:
 */
void
dr1Stream_finit( dr1Stream *str);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method reads at least 'min' characters from the stream, while
 *    attempting to read 'max' characters.  Loops until 'min' characters
 *    are available.
 *
 *    Can return less than min if there is an error on the socket.
 *
 *  PARAMETERS:
 *    str    Stream
 *    buf    Buffer to write to
 *    min    Return with at least this many characters read
 *    max    Try to fill the buffer this full
 *
 *  RETURNS:
 *    Number of characters read.  
 *
 *  SIDE EFFECTS:
 */

int dr1Stream_read( dr1Stream *str, char *buf, int min, int max);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method writes 'len' characters to the stream.
 *
 *  PARAMETERS:
 *    str    Stream
 *    buf    Buffer to write to
 *    len    Length of the buffer
 *
 *  RETURNS:
 *    Number of characters written.  Can return less than 'len' if
 *    there is an error on the stream.
 *
 *  SIDE EFFECTS:
 */

int dr1Stream_write( dr1Stream *str, char *buf, int len);

/*-------------------------------------------------------------------
 * dr1Stream_gets
 * dr1Stream_fgets
 *
 *    The method reads a newline terminated string from the socket.
 *    Characters are read into a buffer and returned one line at a 
 *    time.  There is no absolute limit on line length as the buffer
 *    grows dynamically, but gets() will never return more than size
 *    characters.
 *
 *  PARAMETERS:
 *    str   Stream
 *    buf   Where to put the line that is read
 *    size  Maximum line length to read
 *
 *  RETURNS:
 *    Number of characters placed into the buffer.  On a stream error
 *    the return value is zero unless there is at least one newline
 *    marker in the input buffer.  Once the buffered lines have been
 *    consumed gets() will return 0.  
 *    
 *    Any characters left after the last newline will never be 
 *    returned, but can be examined by looking directly at the buffer
 *    if needed.
 * 
 *    A stream read or write error is saved in the str->error variable
 *    when it occurs, and can be seen immediately or can be examined
 *    only after gets() has returned 0.
 *
 *  SIDE EFFECTS:
 *    The data read from the socket is written to the buf array.
 *
 *    dr1Stream_gets() trims any trailing [CR-LF].  
 *
 *    dr1Stream_fgets() will always return a string with a trailing 
 *    linefeed unless there was a read error from the socket.
 */

int dr1Stream_fgets( dr1Stream *str, char *buf, int size);
int dr1Stream_gets( dr1Stream *str, char *buf, int size);

/*-------------------------------------------------------------------
 * dr1
 *
 *    The method writes a formatted print buffer to the stream.
 *
 *  PARAMETERS:
 *    str    Stream
 *    fmt    Format descriptor in printf format
 *
 *  RETURNS:
 *    Number of characters written.
 *
 *  SIDE EFFECTS:
 */

int dr1Stream_printf( dr1Stream *str, char *fmt, ...);

/*-------------------------------------------------------------------
 * dr1Stream_iqlen( dr1Stream *str);
 *
 *    The method returns the number of characters available in the
 *    input queue
 *
 *  PARAMETERS:
 *    str    Stream
 *
 *  RETURNS:
 *    Number of characters available
 *
 *  SIDE EFFECTS:
 */
int dr1Stream_iqlen( dr1Stream *str);

#endif /* __DR1STREAM__H */
