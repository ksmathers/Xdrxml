#include <stdarg.h>
#include <errno.h>
#include "context.h"

/*-------------------------------------------------------------------
 * dr1_qgets
 *
 *    The method dr1_qgets() returns a newline terminated string from
 *    the socket.
 *
 *  PARAMETERS:
 *    buf    Buffer to write string to
 *    size   Size of the buffer
 *    cs     Socket to read from
 *
 *  RETURNS:
 *    0   on success
 *   -1   file error (feof, or ferror)
 *    1   on failure, and sets errno
 *    errno EWOULDBLOCK    no input currently available
 *
 *  SIDE EFFECTS:
 *    buf gets the string read from the socket
 */
int dr1_qgets( char *buf, int size, dr1Stream *str)
{
    char *nl;
    int len;

    if (!str->inputready) { errno=EWOULDBLOCK; return 1; }
    buf[0] = 0;
    len = dr1Stream_gets(str, buf, size);
    if (len == 0) return -1;

    nl = rindex( buf, '\n');
    if (nl) *nl = 0;
    nl = rindex( buf, '\r');
    if (nl) *nl = 0;

    return 0;
}

/*-------------------------------------------------------------------
 * dr1_qprintf
 *
 *    The method dr1_qprintf() writes a formatted string to the
 *    socket
 *
 *  PARAMETERS:
 *    str  Socket to write to
 *    fmt  Format string (see printf)
 *
 *  RETURNS:
 *    0 on success
 *
 *  SIDE EFFECTS:
 */
int dr1_qprintf( dr1Stream *str, char *fmt, ...) {
    va_list va;
    va_start( va, fmt);
    vfprintf( ctx->fp, fmt, va);
    va_end( va);
    fflush( ctx->fp);
    return 0;
}

