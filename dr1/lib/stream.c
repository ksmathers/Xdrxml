#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "stream.h"
#include "protocol.h"

dr1Stream* dr1Stream_create( dr1Stream *str, int sd) {
    if (!str) str = calloc( 1, sizeof(dr1Stream));
    str->fd = sd;
    dr1StringBuffer_create( &str->ibuf);
    dr1StringBuffer_create( &str->obuf);
    return str;
}

int dr1Stream_read( dr1Stream *str, char *buf, int min, int max) {
    int count = 0;
    int nread;

    do {
	nread = read( str->fd, buf+count, max-count);
/*	printf("stream: Read %d bytes\n", nread); */
	if (nread < 0) {
	    str->error = errno;
	    perror("read");
	    break;
	}
	count += nread;
    } while (count < min);
    return count;
}

int dr1Stream_write( dr1Stream *str, char *buf, int size) {
    int count = 0;
    int nwrite;

    printf("clnt> '%s'\n", buf);
    do {
	nwrite = write( str->fd, buf, size-count);
	if (nwrite < 0) {
	    str->error = errno;
	    perror("write");
	    break;
	}
	count += nwrite;
    } while (count < size);
    return count;
}

int dr1Stream_fgets( dr1Stream *str, char *buf, int size) {
    char tbuf[1024];
    int nread;
    int len;

    /* Buffer ahead to the next newline */
    len = sbindex( &str->ibuf, '\n')+1;
    while (!len) {
	nread = dr1Stream_read( str, tbuf, 1, sizeof( tbuf));
/*	printf("stream: fgets %d bytes total\n", nread); */
	if (nread == 0) {
	    return 0;
	}
	sbcat( &str->ibuf, tbuf, nread);
	len = sbindex( &str->ibuf, '\n')+1;
    }

    /* Got a line, now copy to the buffer */
    if (len >= size) len = size-1;
    memcpy( buf, str->ibuf.buf, len);
    buf[len] = 0;
    sbtail( &str->ibuf, len);
    printf("stream: fgets line is %d bytes\n", len);
    return len;
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
    printf("stream: gets line is %d bytes\n", len);
    return len;
}

int dr1Stream_getdoc( dr1Stream *str, dr1StringBuffer *buf) {
    int len, tlen = 0;
    char line[1024];
    do {
	len = dr1Stream_fgets( str, line, sizeof(line));
	if (strstr( line, SEPARATOR)) break;
	sbcat( buf, line, len);
	tlen += len;
    } while (len > 0);
    return tlen;
}

int dr1Stream_printf( dr1Stream *str, char *fmt, ...) {
    int len;
    va_list va;
    va_start( va, fmt);
    vsbprintf( &str->obuf, fmt, va);
    dr1Stream_write( str, str->obuf.buf, str->obuf.cpos);
    sbclear( &str->obuf);
    va_end( va);
}

int dr1Stream_iqlen( dr1Stream *str) {
    return str->ibuf.cpos;
}
