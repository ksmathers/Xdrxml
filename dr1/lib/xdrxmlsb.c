#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpointer.h>

#include "xdrxml.h"
#include "strbuf.h"

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */

struct xdr_ops xdrxmlsb_ops = {
    	&xdrxml_getlong,
	&xdrxmlsb_putlong,
	&xdrxml_getbytes,
	&xdrxmlsb_putbytes,
	&xdrxml_getpostn,
	&xdrxml_setpostn,
	&xdrxml_inline,
	&xdrxml_destroy,
    	&xdrxml_getint32,
	&xdrxmlsb_putint32
    };

struct xdr_ops_ext xdrxmlsb_extops = {
	&xdrxml_getbit,
	&xdrxmlsb_putbit,
        &xdrxml_getstring,
	&xdrxmlsb_putstring
    };

dr1StringBuffer xdrxmlsb_sb = {
    0,
    0,
    0
};

struct xdrxml_st xdrxmlsb_data = {
    "",
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    &xdrxmlsb_sb,
    &xdrxmlsb_extops
};

#define XDRXML_DATA(xdrs) ((struct xdrxml_st *)xdrs->x_private)

XDR xdrxmlsb = {
    XDR_ENCODE,
    &xdrxmlsb_ops,
    NULL,
    (void *)&xdrxmlsb_data,
    NULL,
    XDR_ANNOTATE
};

int xdr_xml_sb_create( XDR* xdrs, dr1StringBuffer *sb, enum xdr_op xop) {
    struct xdrxml_st *xdrd;

    xdrd = calloc( 1, sizeof(struct xdrxml_st));
    if (xop == XDR_DECODE) {
	/*
	 * build an XML tree from a the file;
	 */
	xdrd->doc = xmlParseDoc( sb->buf);
	assert(xdrd->doc);
	if (xdrd->doc == NULL) return -1;
	xdrd->cur = xmlDocGetRootElement(xdrd->doc);
	if (xdrd->cur == NULL) return -1;
    } 

    if (xop == XDR_ENCODE) {
	/* 
	 * Open file for writing
	 */
	xdrd->sb = calloc( 1, sizeof( dr1StringBuffer));
	if (xdrd->sb == NULL) return -1;
    }

    xdrs->x_op = xop;
    xdrs->x_ops = &xdrxmlsb_ops;
    xdrs->x_public = NULL;
    xdrs->x_private = (void *)xdrd;
    xdrs->x_base = NULL;
    xdrs->x_handy = XDR_ANNOTATE;

    return 0;
}

bool_t xdrxmlsb_putstring( XDR *xdrs, char *s) {
    /* put some bytes to " */
    dr1StringBuffer *sb;
    int i;
    unsigned char c;
    int len;
    char *attr = XDRXML_DATA(xdrs)->attr;
    int ni = nchar( XDRXML_DATA(xdrs)->path, '/');
    struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);

    sb = xdrd->sb;

    while (ni--) sbprintf(sb, "<%s>", attr);
    len = strlen( s);
    for (i=0; i<len; i++) {
	c = s[ i];
	if (c >=' ' && c <='~' && c != '=' && c != '&' && c != '<') {
	    sbputc( sb, c);
	} else {
	    sbprintf( sb, "=%02x", c);
	}
    } /* for */
    sbprintf(sb, "</%s>\n", attr);

    XDRXML_DATA(xdrs)->attr = 0;
    return TRUE;
}

bool_t xdrxmlsb_putbit( XDR *xdrs, bool_t bit) {
    /* put a bit to the stream */
    dr1StringBuffer* sb;
    char *attr = XDRXML_DATA(xdrs)->attr;
    int ni = nchar( XDRXML_DATA(xdrs)->path, '/');

    if (!bit) return TRUE;
    sb = XDRXML_DATA(xdrs)->sb;
    if (!attr) attr = "bit";
    
    while (ni--) sbprintf(sb, "    ");
    sbprintf(sb, "<%s/>\n", attr);
    return TRUE;
}


bool_t xdrxmlsb_putlong( XDR *__xdrs, __const long *__lp)
{
    /* put a long to " */
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    dr1StringBuffer *sb;
    char *attr = xdrd->attr;
    int ni = nchar( xdrd->path, '/');

    sb = XDRXML_DATA(__xdrs)->sb;
    if (!attr) attr = "long";

    while (ni--) sbprintf(sb, "    ");
    sbprintf(sb, "<%s value=\"0x%lx\"/>\n", attr, *__lp);
    xdrd->attr = NULL;
    return TRUE;
}


bool_t xdrxmlsb_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len)
{
    /* put some bytes to " */
    dr1StringBuffer *sb;
    int i;
    unsigned char c;
    char *attr = XDRXML_DATA(__xdrs)->attr;
    int ni = nchar( XDRXML_DATA(__xdrs)->path, '/');

    sb = XDRXML_DATA(__xdrs)->sb;
    if (!attr) attr = "bytes";
    
    while (ni--) sbprintf(sb, "    ");
    sbprintf(sb, "<%s>", attr);
    for (i=0; i<__len; i++) {
	c = __addr[ i];
	if (c >=' ' && c <='~' && c != '=' && c != '&' && c != '<') {
	    sbputc( sb, c);
	} else {
	    sbprintf(sb, "=%02x", c);
	}
    } /* for */
    sbprintf(sb, "</%s>\n", attr);

    XDRXML_DATA(__xdrs)->attr = 0;
    return TRUE;
}

bool_t xdrxmlsb_putint32( XDR *__xdrs, __const int32_t *__ip)
{
    /* put a int to stream */
    dr1StringBuffer *sb;
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    char *attr = xdrd->attr;
    int ni = nchar( xdrd->path, '/');

    if (!attr) attr = "long";
    sb = xdrd->sb;

    while (ni--) sbprintf(sb, "    ");
    sbprintf(sb, "<%s value=\"0x%lx\"/>\n", attr, (long)*__ip);
    xdrd->attr = NULL;
    return TRUE;
}
