#include "strbuf.h"
#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif

int
dr1StringBuffer_grow( dr1StringBuffer *sb, int len) {
    sb->buf = realloc( sb->buf, sb->bufsize + len);
    assert(sb->buf);
    sb->bufsize += len;
    return 0;
}

int
sbprintf( dr1StringBuffer *sb, char *fmt, ...) {
    int len;
    va_list va;
    va_start( va, fmt);
    len = vsnprintf( sb->buf + sb->cpos, sb->bufsize - sb->cpos, fmt, va);
    if (len < 0 || sb->cpos+len >= sb->bufsize) {
        dr1StringBuffer_grow( sb, min(len, 1024));
	if (!sb->buf) return -1;
	len = vsnprintf( sb->buf + sb->cpos, sb->bufsize - sb->cpos, fmt, va);
    }
    sb->cpos += len;
    assert( sb->cpos < sb->bufsize);
    return len;
}

int 
sbputc( dr1StringBuffer *sb, char c) {
    if (sb->cpos + 1 >= sb->bufsize) {
	dr1StringBuffer_grow( sb, 1024);
	if (!sb->buf) return -1;
    }
    sb->buf[sb->cpos++] = c;
    sb->buf[sb->cpos] = 0;
    return 0;
}

