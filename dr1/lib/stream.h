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
 * dr1Stream_read
 *
 *    Unbuffered reader
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
 * dr1Stream_write
 *
 *    The method writes 'len' characters to the stream.
 * 
 *    This method is unbuffered.  It may not write all of the 
 *    characters requested depending on how much room is 
 *    available in the output stream.  Use buffered output if
 *    you need to write data and don't want to loop until
 *    the whole buffer has been sent.
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
 * dr1Stream_flush( str);
 *
 *
 *  PARAMETERS:
 *    str    Stream
 *
 *  RETURNS:
 *    Number of characters remaining to be written, or -1 on 
 *    error.
 *
 *  SIDE EFFECTS:
 *    Removes characters from the output buffer.
 */
int dr1Stream_flush( dr1Stream* str);
/*-------------------------------------------------------------------
 * dr1Stream_fwrite
 *
 *    The method writes 'len' characters to the stream.
 * 
 *    This method is buffered.  If the stream can't write
 *    all of the data in one request the remaining data
 *    is saved in the output buffer and the request should 
 *    be completed by calling dr1Stream_flush() until it
 *    returns 0.
 *
 *  PARAMETERS:
 *    str    Stream
 *    buf    Buffer to write to
 *    len    Length of the buffer
 *
 *  RETURNS:
 *    Number of characters written.  Returns -1 on error.
 *
 *  SIDE EFFECTS:
 *    Adds characters to the output buffer.
 */

int dr1Stream_fwrite( dr1Stream *str, char *buf, int len);

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
 * dr1Stream_getdoc( str, buf)
 *
 *    The method reads lines until it gets an EOF marker.  The EOF
 *    marker it uses is defined as SEPARATOR in the protocol.h
 *    header.
 *
 *  PARAMETERS:
 *    str   Stream
 *    buf   Where to put the document that is read
 *
 *  RETURNS:
 *    The number of characters read.  
 *
 *  SIDE EFFECTS:
 *    The data read from the socket is written to the buf array.
 *
 *  BUGS:
 *    No error handling.
 */
int dr1Stream_getdoc( dr1Stream *str, dr1StringBuffer *buf);

/*-------------------------------------------------------------------
 * dr1Stream_printf
 *
 *    The method writes a formatted print buffer to the stream.
 *
 *    This method buffers output.  If the entire buffer cannot
 *    be written without blocking then the remainder will be 
 *    saved in the buffer and output when dr1Stream_run is 
 *    called.
 *
 *  PARAMETERS:
 *    str    Stream
 *    fmt    Format descriptor in printf format
 *
 *  RETURNS:
 *    Number of characters written.  Unwritten characters are
 *    queued for later output.  Returns -1 if there is an error.
 *
 *  SIDE EFFECTS:
 */

int dr1Stream_printf( dr1Stream *str, char *fmt, ...);

/*-------------------------------------------------------------------
 * dr1Stream_iqlen( dr1Stream *str);
 * dr1Stream_oqlen( dr1Stream *str);
 *
 *    The method returns the number of characters available in the
 *    input queue, and output queue respectively.
 *
 *  PARAMETERS:
 *    str    Stream
 *
 *  RETURNS:
 *    Number of characters available, or waiting to be written.
 *
 *  SIDE EFFECTS:
 */
int dr1Stream_iqlen( dr1Stream *str);
int dr1Stream_oqlen( dr1Stream *str);

#endif /* __DR1STREAM__H */
