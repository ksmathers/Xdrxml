#include <stdarg.h>
#include <string.h>
#include "protocol.h"
#include "strbuf.h"
#include "stream.h"

int pisMessage( char *buf, char *msg) {
    int differ = strncmp( buf, msg, 3);
    return !differ;
}

static void 
psendUrlEncodedChar( dr1StringBuffer *sb, char c) {
    if (c == ' ') {
	sbputc( sb, '+');
    } else if (c == '+' || c == '%' || c < 32 || c >=127) {
	sbprintf( sb, "%%%02X", (int)c);
    } else {
	sbputc( sb, c);
    }
}

static char 
precvUrlEncodedChar( char **posn) {
    char *cpos = *posn;
    char buf[3];
    char c;
    if (*cpos == '+') { 
	c=' ';
    } else if (*cpos == '%') {
	buf[0] = *++cpos;
	buf[1] = *++cpos;
	buf[2] = 0;
	c = strtol( buf, NULL, 16);
    } else {
        c = *cpos;
    }
    *posn = cpos;
    return c;
}

static void
psendUrlEncoded( dr1StringBuffer* sb, char *str) {
    char *cpos = str;
    while (*cpos) {
        psendUrlEncodedChar( sb, *cpos);
        cpos++;
    }
}

static void
precvUrlEncoded( char **posn, dr1StringBuffer *string) {
    char *cpos = *posn;
    int c; 
    char buf[3];
    while (*cpos && *cpos > 32 && *cpos < 127 ) {
        c = precvUrlEncodedChar( &cpos);
	sbputc( string, c);
        cpos++;
    }
    *posn = cpos;
}

void
psendMessage( dr1Stream* os, char *msg, ...) {
    dr1StringBuffer* sb = &os->obuf;
    char *cpos = msg;
    char *sparm;
    int dparm;
    va_list va;
    va_start(va, msg);
    while (*cpos) {
        if (*cpos == '%') {
            cpos++;
            switch (*cpos) {
                case 'c':
  		    dparm = va_arg( va, int);
		    psendUrlEncodedChar( sb, dparm);
  		    break;
                case 's':
  		    sparm = va_arg( va, char *);
		    psendUrlEncoded( sb, sparm);
  		    break;
                case 'd':
		    dparm = va_arg( va, int);
		    sbprintf( sb, "%d", dparm);
                    break;
            }
        } else {
            sbputc( sb, *cpos);
        }
        cpos++;
    } /* while */
    sbputc( sb, '\n');
    dr1Stream_flush( os);
    va_end( va);
}

int
precvMessage( char *buf, char *msg, ...) {
    va_list va;
    char *inpos = buf;
    char *fmpos = msg;
    dr1StringBuffer stmp;
    char **sarg;
    int *darg;
    char *carg;
    int count=0;

    dr1StringBuffer_create( &stmp);
    va_start( va, msg);

    while (*fmpos && *inpos) {
        if (*fmpos == '%') {
	    fmpos++;
	    switch (*fmpos) {
		case 'c':
		    count++;
		    carg = va_arg( va, char *);
		    *carg = precvUrlEncodedChar( &inpos);
		    break;
		case 's':
		    count++;
                    sbclear( &stmp);
		    sarg = va_arg( va, char **);
		    precvUrlEncoded( &inpos, &stmp);
                    *sarg = strdup( stmp.buf);
		    break;
		case 'd':
		    count++;
                    darg = va_arg( va, int *);
		    *darg = strtol( inpos, &inpos, 10);
		    break;
	    }
	} else {
	    if (*fmpos != *inpos) break;
	    inpos++;
        } 
	fmpos++;
    }
    dr1StringBuffer_finit( &stmp);
    va_end( va);
    return count;
}
