#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpointer.h>

#include "xdrxml.h"

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */

struct xdr_ops xdrxml_ops = {
    	&xdrxml_getlong,
	&xdrxml_putlong,
	&xdrxml_getbytes,
	&xdrxml_putbytes,
	&xdrxml_getpostn,
	&xdrxml_setpostn,
	&xdrxml_inline,
	&xdrxml_destroy,
    	&xdrxml_getint32,
	&xdrxml_putint32
    };

struct xdrxml_st {
        char path[1024];
	char *attr;
	FILE *fp;		/* output stream */
	xmlDocPtr doc;		/* input stream */
	xmlNodePtr cur;		/* input cursor */
	xmlNodePtr last;	/* last node used */
    };

struct xdrxml_st xdrxml_data = {
    "",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

#define XDRXML_DATA(xdrs) ((struct xdrxml_st *)xdrs->x_private)

XDR xdrxml = {
    XDR_ENCODE,
    &xdrxml_ops,
    NULL,
    (void *)&xdrxml_data,
    NULL,
    XDR_ANNOTATE
};

/*
 * utility functions 
 */
static void mark( xmlNodePtr node) {
    ((char *)node->name)[0] = 0;
}

static xmlNodePtr bfs1( xmlNodePtr node, const char *name) {
    xmlNodePtr cur = node->children;
    while (cur != NULL && strcasecmp( cur->name, name) != 0) {
    	cur = cur->next;
    }
    return cur;
}

static int nchar( char *path, int c) {
    int i=0;
    while (*path) if (*path++ == c) i++;
    return i;
}

int xdr_xml_create( XDR* xdrs, char *fname, enum xdr_op xop) {
    struct xdrxml_st *xdrd;

    xdrd = calloc( 1, sizeof(struct xdrxml_st));
    if (xop == XDR_DECODE) {
	/*
	 * build an XML tree from a the file;
	 */
	printf("Parsing %s\n", fname);
	xdrd->doc = xmlParseFile( fname);
	assert(xdrd->doc);
	printf("Doc loaded\n");
	if (xdrd->doc == NULL) return -1;
	xdrd->cur = xmlDocGetRootElement(xdrd->doc);
	if (xdrd->cur == NULL) return -1;
    } 

    if (xop == XDR_ENCODE) {
	/* 
	 * Open file for writing
	 */
	xdrd->fp = fopen( fname, "w");
	if (xdrd->fp == NULL) return -1;
    }

    xdrs->x_op = xop;
    xdrs->x_ops = &xdrxml_ops;
    xdrs->x_public = NULL;
    xdrs->x_private = (void *)xdrd;
    xdrs->x_base = NULL;
    xdrs->x_handy = XDR_ANNOTATE;

    return 0;
}

bool_t xdr_push_note( XDR *xdrs, const char *s)
{
    if (xdrs->x_handy == XDR_ANNOTATE) {
        char buf[80];
        struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
	int ni = nchar( xdrd->path, '/');

        /* add node to the path */
	sprintf(buf, "/%s", s);
	strcat( xdrd->path, buf);

	/* process the node */
	if (xdrs->x_op == XDR_ENCODE) {
	    FILE *fp = xdrd->fp;

	    if (!fp) fp = stdout;
	    while (ni--) fprintf(fp, "    ");
	    fprintf( fp, "<%s>\n", s); 
	}

	if (xdrs->x_op == XDR_DECODE) {
	    xmlNodePtr cur;
	    cur = bfs1( xdrd->cur, s);
	    if (!cur) return FALSE;
	    xdrd->cur = cur;
	}
    }
    return TRUE;
}

bool_t xdr_pop_note( XDR *xdrs) 
{
    if (xdrs->x_handy & XDR_ANNOTATE) {
        char *cpos;
        struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);

        /* remove the node from the path */
	cpos = rindex( xdrd->path, '/');
	if (cpos) { *cpos = 0; cpos++; }

	/* process the node */
	if (xdrs->x_op == XDR_ENCODE) {
	    FILE *fp = xdrd->fp;
	    int ni = nchar( xdrd->path, '/');

	    if (!fp) fp = stdout;

	    while (ni--) fprintf(fp, "    ");
	    fprintf( fp, "</%s>\n", cpos); 
	}
	if (xdrs->x_op == XDR_DECODE) {
	    xmlNodePtr cur;
	    cur = xdrd->cur->parent;
	    if (!cur) return FALSE;
	    mark(xdrd->cur);
	    xdrd->cur = cur;
	}
    }
    return TRUE;
}

void xdr_attr( XDR *xdrs, const char *s) 
{
    if (xdrs->x_handy & XDR_ANNOTATE) {
	XDRXML_DATA(xdrs)->attr = strdup(s);
    }
}

bool_t xdrxml_getlong( XDR *__xdrs, long *__lp)
{
    /* get a long from underlying stream */
    xmlNodePtr cur = XDRXML_DATA(__xdrs)->cur;
    char *attr = XDRXML_DATA(__xdrs)->attr;
    char *value;
    char *err;

    if (!attr) attr="long";

    cur = bfs1( cur, attr);
    if (!cur) return FALSE;

    value = xmlGetProp(cur, "value");
    if (!value) return FALSE;

    *__lp = strtol( value, &err, 0);
    if (*err != 0) {
        printf("Badly formatted long at '%s'='%s'\n", attr, value);
	return FALSE;
    }
    mark(cur);

    return TRUE;
}

bool_t xdrxml_putlong( XDR *__xdrs, __const long *__lp)
{
    /* put a long to " */
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    FILE *fp;
    char *attr = xdrd->attr;
    int ni = nchar( xdrd->path, '/');

    if (!attr) attr = "long";
    if (xdrd->fp) fp = xdrd->fp;
    else fp = stdout;

    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s value=\"0x%lx\"/>\n", attr, *__lp);
    xdrd->attr = NULL;
    return TRUE;
}


bool_t xdrxml_getbytes( XDR *__xdrs, caddr_t __addr,
			     u_int __len)
{
    /* get some bytes from " */
    xmlNodePtr cur = XDRXML_DATA(__xdrs)->cur;
    xmlDocPtr doc = XDRXML_DATA(__xdrs)->doc;
    char *attr = XDRXML_DATA(__xdrs)->attr;
    char *value, *cpos, *err;
    int c;
    int i;

    if (!attr) attr="bytes";

    cur = bfs1( cur, attr);
    if (!cur) return FALSE;
    mark(cur);
    value = xmlNodeListGetString(doc, cur->children, 1);
    if (!value) return FALSE;

    cpos = value;
    for (i=0; i<__len; i++) {
	c = *cpos++;
	if (c == 0) {
	    printf( "String should be %d chars long: '%s'\n", __len, value);
	    return FALSE;
	}
	if (c != '=') {
	    __addr[i] = c;
	} else {
	    char buf[3];
	    buf[0] = *cpos++;
	    buf[1] = *cpos++;
	    buf[2] = 0;
	    c = strtol(buf, &err, 16);
	    if (*err != 0) {
	        printf( "Invalid escaped hex value '=%s' in string '%s'\n", 
			buf, value);
		return FALSE;
	    }
	    __addr[i] = c;
	}
    }
    return TRUE;
}

bool_t xdrxml_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len)
{
    /* put some bytes to " */
    FILE *fp;
    int i;
    unsigned char c;
    char *attr = XDRXML_DATA(__xdrs)->attr;
    int ni = nchar( XDRXML_DATA(__xdrs)->path, '/');

    fp = XDRXML_DATA(__xdrs)->fp;
    if (!fp) fp = stdout;
    if (!attr) attr = "bytes";
    
    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s>", attr);
    for (i=0; i<__len; i++) {
	c = __addr[ i];
	if (c >=' ' && c <='~' && c != '=' && c != '&' && c != '<') {
	    putc( c, fp);
	} else {
	    fprintf(fp, "=%02x", c);
	}
    } /* for */
    fprintf(fp, "</%s>\n", attr);

    XDRXML_DATA(__xdrs)->attr = 0;
    return TRUE;
}

u_int xdrxml_getpostn( __const XDR *__xdrs)
{
    /* returns bytes off from beginning */
    FILE *fp;
    fp = XDRXML_DATA(__xdrs)->fp;
    assert(fp);
    return ftell(fp);
}

bool_t xdrxml_setpostn( XDR *__xdrs, u_int pos)
{
    /* lets you reposition the stream */
    FILE *fp;
    fp = XDRXML_DATA(__xdrs)->fp;
    assert(fp);
    return fseek(fp, pos, SEEK_SET);
}

int32_t *xdrxml_inline( XDR *__xdrs, int len)
{
    /* buf quick ptr to buffered data */
    assert(1==0);
    return NULL;
}

void xdrxml_destroy( XDR *__xdrs)
{
    /* free privates of this xdr_stream */
    struct xdrxml_st* xdrd = XDRXML_DATA(__xdrs);
    if (xdrd->fp) fclose(xdrd->fp);
   /* if (xdrd->doc) FIXME /**/

}

bool_t xdrxml_getint32( XDR *__xdrs, int32_t *__ip)
{
    /* get a int from underlying stream */
    xmlNodePtr cur = XDRXML_DATA(__xdrs)->cur;
    char *attr = XDRXML_DATA(__xdrs)->attr;
    char *value;
    char *err;

    if (!attr) attr="long";

    cur = bfs1( cur, attr);
    if (!cur) return FALSE;

    value = xmlGetProp(cur, "value");
    if (!value) return FALSE;

    *__ip = (int32_t)strtol( value, &err, 0);
    if (*err != 0) {
        printf("Badly formatted long at '%s'='%s'\n", attr, value);
	return FALSE;
    }
    mark(cur);

    return TRUE;
}

bool_t xdrxml_putint32( XDR *__xdrs, __const int32_t *__ip)
{
    /* put a int to stream */
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    FILE *fp;
    char *attr = xdrd->attr;
    int ni = nchar( xdrd->path, '/');

    if (!attr) attr = "long";
    if (xdrd->fp) fp = xdrd->fp;
    else fp = stdout;

    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s value=\"0x%lx\"/>\n", attr, (long)*__ip);
    xdrd->attr = NULL;
    return TRUE;
}
