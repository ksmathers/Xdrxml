#include <string.h>
#include "strbuf.h"
#ifndef max
#define max(x,y) ((x)>(y)?(x):(y))
#endif

dr1StringBuffer*
dr1StringBuffer_create( dr1StringBuffer* _sb) {
    dr1StringBuffer *sb;

    if (_sb) {
	sb = _sb;
    } else {
	sb = malloc( sizeof( dr1StringBuffer));
    }
    bzero( sb, sizeof( dr1StringBuffer));

    return sb;
}

int
dr1StringBuffer_grow( dr1StringBuffer *sb, int len) {
    sb->bufsize += len;
    sb->buf = realloc( sb->buf, sb->bufsize);
    assert(sb->buf);
    return 0;
}

void
sbclear( dr1StringBuffer* sb) {
    free( sb->buf);
    sb->buf = NULL;
    sb->bufsize = 0;
    sb->cpos = 0;
}

int
vsbprintf( dr1StringBuffer *sb, char *fmt, va_list va) {
    int len;
    len = vsnprintf( sb->buf + sb->cpos, sb->bufsize - sb->cpos, fmt, va);
    if (len < 0 || sb->cpos+len >= sb->bufsize) {
        dr1StringBuffer_grow( sb, max(len+1, 1024));
	if (!sb->buf) return -1;
	len = vsnprintf( sb->buf + sb->cpos, sb->bufsize - sb->cpos, fmt, va);
    }
    sb->cpos += len;
    assert( sb->cpos < sb->bufsize);
    return len;
}

int
sbprintf( dr1StringBuffer *sb, char *fmt, ...) {
    int len;
    va_list va;
    va_start( va, fmt);
    len = vsbprintf( sb, fmt, va);
    va_end( va);
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

int
sbstrcat( dr1StringBuffer *sb, char *str) {
    return sbcat( sb, str, strlen(str));
}

int
sbcat( dr1StringBuffer *sb, char *str, int len) {
    if (sb->cpos + len >= sb->bufsize) {
	dr1StringBuffer_grow( sb, max( len+1, 1024));
	if (!sb->buf) return -1;
    }
    memcpy(sb->buf + sb->cpos, str, len);
    sb->cpos += len;
    sb->buf[sb->cpos] = 0;
    return 0;
}

int
sbtail( dr1StringBuffer *sb, int start_char) {
    memcpy( sb->buf, sb->buf + start_char, sb->cpos - start_char);
    sb->buf[sb->cpos] = 0;
    sb->cpos -= start_char;
}

int 
sbindex( dr1StringBuffer *sb, int ch) {
    int count=0;
    char *s = sb->buf;
    while (count < sb->cpos && *s != ch) s++, count++;
    if (count < sb->cpos) return count;
    return -1;
}
