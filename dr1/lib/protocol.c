#include <stdarg.h>
#include <string.h>
#include "protocol.h"
#include "strbuf.h"
#include "stream.h"

static void
psendUrlEncoded( dr1StringBuffer* sb, char *str) {
    char *cpos = str;
    while (*cpos) {
        if (*cpos == ' ') {
	    sbputc( sb, '+');
        } else if (*cpos == '+' || *cpos == '%' || *cpos < 32 || *cpos >=127) {
            sbprintf( sb, "%%%02X", (int)*cpos);
        } else {
            sbputc( sb, *cpos);
        }
        cpos++;
    }
}

static void
precvUrlEncoded( char **posn, dr1StringBuffer *string) {
    char *cpos = *posn;
    int c; 
    char buf[3];
    while (*cpos && *cpos > 32 && *cpos < 127 ) {
        if (*cpos == '+') { 
            sbputc( string, ' ');
        } else if (*cpos == '%') {
            buf[0] = *++cpos;
            buf[1] = *++cpos;
            buf[2] = 0;
            c = strtol( buf, NULL, 16);
            sbputc( string, c);
        } else {
            sbputc( string, *cpos);
        }
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
    int count=0;

    dr1StringBuffer_create( &stmp);
    va_start( va, msg);

    while (*fmpos && *inpos) {
        if (*fmpos == '%') {
	    fmpos++;
	    switch (*fmpos) {
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
