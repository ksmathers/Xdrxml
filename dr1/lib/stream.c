#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "stream.h"

dr1Stream* dr1Stream_create( dr1Stream *str, int sd) {
    if (!str) str = calloc( 1, sizeof(dr1Stream));
    str->fd = sd;
    dr1StringBuffer_create( &str->buf);
    return str;
}

int dr1Stream_read( dr1Stream *str, char *buf, int min, int max) {
    int count = 0;
    int nread;

    do {
	nread = read( str->fd, buf, max-count);
	if (nread < 0) {
	    str->error = errno;
	    perror("read");
	    break;
	}
	count += nread;
    } while (count < min);
    return count;
}

int dr1Stream_fgets( dr1Stream *str, char *buf, int size) {
    char tbuf[1024];
    int nread;
    char *cpos;

    /* Buffer ahead to the next newline */
    cpos = index( str->buf.buf, '\n');
    while (!cpos) {
	nread = dr1Stream_read( str, tbuf, 1, sizeof( tbuf));
	if (nread == 0) {
	    cpos = str->buf.buf + str->buf.cpos;
	    break;
	}
	sbcat( &str->buf, tbuf, nread);
	cpos = index( str->buf.buf, '\n');
    }

    /* Got a line, now copy to the buffer */
    nread = cpos - str->buf.buf;
    if (nread >= size) nread = size-1;
    memcpy( buf, str->buf.buf, nread);
    buf[nread] = 0;
    sbtail( &str->buf, nread);
    return nread;
}

int dr1Stream_gets( dr1Stream *str, char *buf, int size) {
    int len;
    len = dr1Stream_fgets( str, buf, size);
    if (len == 0) return -1;
    if (buf[len-1] == '\n') {
	buf[--len] = 0;
    }
    if (buf[len-1] == '\r') {
        buf[--len] = 0;
    }
    return len;
}

int dr1Stream_printf( dr1Stream *str, char *fmt, ...) {
    va_list va;
    va_start( va, fmt);
    vsbprintf( &str->buf, fmt, va);
    va_end( va);
}
