#ifndef __DR1QIO__H
#define __DR1QIO__H

#define qgets dr1_qgets
#define qprintf dr1_qprintf
/*-------------------------------------------------------------------
 * dr1_qgets
 *
 *    The method dr1_qgets() returns a newline terminated string from
 *    the client context
 *
 *  PARAMETERS:
 *    buf    Buffer to write string to
 *    size   Size of the buffer
 *    ctx    Client context
 *
 *  RETURNS:
 *    0   on success
 *
 *  SIDE EFFECTS:
 *    buf gets the string read from the socket
 */
int dr1_qgets( char *buf, int size, dr1Context* ctx);

/*-------------------------------------------------------------------
 * dr1_qprintf
 *
 *    The method dr1_qprintf() writes a formatted string to the
 *    client context
 *
 *  PARAMETERS:
 *    ctx  Client context
 *    fmt  Format string (see printf)
 *
 *  RETURNS:
 *    0 on success
 *
 *  SIDE EFFECTS:
 */
int dr1_qprintf( dr1Context* ctx, char *fmt, ...);

#endif /* __DR1QIO__H */

