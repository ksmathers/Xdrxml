#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include "xdrasc.h"

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */

struct xdr_ops xdrasc_ops = {
    	&xdrasc_getlong,
	&xdrasc_putlong,
	&xdrasc_getbytes,
	&xdrasc_putbytes,
	&xdrasc_getpostn,
	&xdrasc_setpostn,
	&xdrasc_inline,
	&xdrasc_destroy,
    	&xdrasc_getint32,
	&xdrasc_putint32
    };

struct xdrasc_st {
	struct annnode *head;
	char *attr;
    };

struct xdrasc_st xdrasc_data = {
    NULL,
    NULL
};

#define XDRASC_DATA(xdrs) ((struct xdrasc_st *)xdrs->x_private)

XDR xdrasc = {
    XDR_ENCODE,
    &xdrasc_ops,
    NULL,
    (void *)&xdrasc_data,
    NULL,
    XDR_ANNOTATE
};

struct annnode {
    struct annnode *next;
    char *note;
};

void xdr_push_note( XDR *xdrs, const char *s, ...)
{
    va_list va;
    va_start( va, s);
    if (xdrs->x_handy & XDR_ANNOTATE) {
	char buf[80];
        struct annnode *a = calloc( 1, sizeof( *a));

        vsprintf(buf, s, va);
	a->next = (struct annnode*)XDRASC_DATA(xdrs)->head;
	a->note = strdup(buf);
	XDRASC_DATA(xdrs)->head = (void *)a;

	printf("<%s>\n", a->note);
    }
}

void xdr_pop_note( XDR *xdrs) 
{
    if (xdrs->x_handy & XDR_ANNOTATE) {
	struct annnode *a = XDRASC_DATA(xdrs)->head;
	XDRASC_DATA(xdrs)->head = (void *)a->next;

	printf("</%s>\n", a->note);
	free(a->note);
	free(a);
    }
}

void xdr_attr( XDR *xdrs, const char *s, ...) {
    va_list va;
    va_start( va, s);
    if (xdrs->x_handy & XDR_ANNOTATE) {
	char buf[80];
        vsprintf(buf, s, va);
	
	XDRASC_DATA(xdrs)->attr = strdup(buf);
    }
}

bool_t xdrasc_getlong( XDR *__xdrs, long *__lp)
{
    /* get a long from underlying stream */
    assert(1==0);
}

bool_t xdrasc_putlong( XDR *__xdrs, __const long *__lp)
{
    /* put a long to " */
    printf("<%s>", XDRASC_DATA(__xdrs)->attr);
    printf("0x%lx", *__lp);
    printf("</%s>\n", XDRASC_DATA(__xdrs)->attr);
}

bool_t xdrasc_getbytes( XDR *__xdrs, caddr_t __addr,
			     u_int __len)
{
    /* get some bytes from " */
    assert(1==0);
}

bool_t xdrasc_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len)
{
    /* put some bytes to " */
    int i;
    char c;
    printf("<%s>", XDRASC_DATA(__xdrs)->attr);

    for (i=0; i<__len; i++) {
	c = __addr[ i];
	if (c >=' ' && c <'~') putchar( c);
	else {
	   printf("~%02x", c);
	}
    }
    printf("</%s>\n", XDRASC_DATA(__xdrs)->attr);
}

u_int xdrasc_getpostn( __const XDR *__xdrs)
{
    /* returns bytes off from beginning */
    assert(1==0);
}

bool_t xdrasc_setpostn( XDR *__xdrs, u_int pos)
{
    /* lets you reposition the stream */
    assert(1==0);
}

int32_t *xdrasc_inline( XDR *__xdrs, int len)
{
    /* buf quick ptr to buffered data */
    assert(1==0);
    return NULL;
}

void xdrasc_destroy( XDR *__xdrs)
{
    /* free privates of this xdr_stream */
    assert(1==0);
}

bool_t xdrasc_getint32( XDR *__xdrs, int32_t *__ip)
{
    /* get a int from underlying stream */
    assert(1==0);
}

bool_t xdrasc_putint32( XDR *__xdrs, __const int32_t *__ip)
{
    /* put a int to " */
    printf("<%s>", XDRASC_DATA(__xdrs)->attr);
    printf("0x%x", *__ip);
    printf("</%s>\n", XDRASC_DATA(__xdrs)->attr);
}
