#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

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

struct xdr_ops_ext xdrxml_extops = {
	&xdrxml_getbit,
	&xdrxml_putbit,
        &xdrxml_getstring,
	&xdrxml_putstring,
	&xdrxml_printf
    };


struct xdrxml_st xdrxml_data = {
    "",
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &xdrxml_extops
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
void xdrxml_mark( xmlNodePtr node) {
/*    printf("mark %s\n", node->name); /**/
    ((char *)node->name)[0] = 0;
}

xmlNodePtr xdrxml_bfs1( xmlNodePtr node, const char *name) {
    xmlNodePtr cur = node->children;
    while (cur != NULL && strcasecmp( cur->name, name) != 0) {
    	cur = cur->next;
    }
/*    printf("%s %s\n", cur?"found":"lost", name); /**/
    return cur;
}

int xdrxml_nchar( char *path, int c) {
    int i=0;
    while (*path) if (*path++ == c) i++;
    return i;
}

void xdrxml_free( xdrxmlproc_t proc, void *data) {
    XDR xdrs;
    bzero( &xdrs, sizeof(xdrs));
    xdrs.x_op = XDR_FREE;
    proc( &xdrs, NULL, data);
}

int xdr_xml_create( XDR* xdrs, char *fname, enum xdr_op xop) {
    struct xdrxml_st *xdrd;

    xdrd = calloc( 1, sizeof(struct xdrxml_st));
    xdrd->ext = &xdrxml_extops;
    if (xop == XDR_DECODE) {
	/*
	 * build an XML tree from a the file;
	 */
/*	printf("Parsing file %s\n", fname); /**/
	xdrd->doc = xmlParseFile( fname);
/*	printf("done\n"); /**/
	assert(xdrd->doc);
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

int xdrxml_printf( XDR *xdrs, char *fmt, ...) {
    FILE *fp = XDRXML_DATA(xdrs)->fp;
    int res;
    va_list va;
    va_start( va, fmt);

    if (!fp) fp = stdout;
    res = vfprintf( fp, fmt, va);
    va_end( va);
    return res;
}
int xdrxml_error( XDR *xdrs) {
    /* get some bytes from " */
    if (xdrs->x_handy & XDR_ANNOTATE) {
       /* xml stream */
       struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
       return xdrd->error;
    }
    return -1;
}

void xdrxml_clearerr( XDR *xdrs) {
    /* get some bytes from " */
    if (xdrs->x_handy & XDR_ANNOTATE) {
       /* xml stream */
       struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
       xdrd->error = DR1_ENOERROR;
    }
}

bool_t xdrxml_bool( XDR *xdrs, char *node, bool_t *bool) {
    bool_t res;

    xdr_attr( xdrs, node);
    if (xdrs->x_handy & XDR_ANNOTATE) {
	/* xml stream */
	struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
        if (xdrs->x_op == XDR_DECODE) {
	    res = xdrd->ext->x_getbit( xdrs, bool);
	} else if (xdrs->x_op == XDR_ENCODE) {
	    res = xdrd->ext->x_putbit( xdrs, *bool);
	}
    } else {
	res = xdr_bool( xdrs, bool);
    }
    return res;
}


bool_t xdrxml_arraysize( XDR *xdrs, char *node, int *size) {
    struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
    bool_t res;
    if (xdrs->x_handy & XDR_ANNOTATE) {
	/* annotated stream */
	*size = xdrxml_count_children( xdrs, node);
	res = TRUE;
    } else {
        res = xdr_int( xdrs, size);
    }
    return res;
}

int xdrxml_count_children( XDR *xdrs, char *node) {
    struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
    xmlNodePtr cur = xdrd->cur;
    xmlNodePtr c;
    int count = 0;
    for (c=cur->children; c != NULL; c = c->next) {
        if (!strcmp(c->name, node)) count++;
	if (count > 500) { count=0; break; }
	if (c == cur->last) break;
    }
    return count;
}

bool_t xdrxml_int32_t( XDR *xdrs, char *node, int32_t *data) {
    bool_t res;
    xdr_attr( xdrs, node);
    res = xdr_int32_t( xdrs, data);
    return res;
}

bool_t xdrxml_int( XDR *xdrs, char *node, int *data) {
    bool_t res;
    xdr_attr( xdrs, node);
    res = xdr_int( xdrs, data);
    return res;
}

bool_t xdrxml_long( XDR *xdrs, char *node, long *data) {
    bool_t res;
    xdr_attr( xdrs, node);
    res = xdr_long( xdrs, data);
    return res;
}

bool_t xdrxml_wrapstring( XDR *xdrs, char *node, char **s) 
{
    bool_t res;
    /* send or read a string from the stream */
    xdr_attr( xdrs, node);
    if (xdrs->x_handy & XDR_ANNOTATE) {
	/* xml stream */
	struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
	if (xdrs->x_op == XDR_ENCODE) {
	    res = xdrd->ext->x_putstring( xdrs, *s);
	} else if (xdrs->x_op == XDR_DECODE) {
	    res = xdrd->ext->x_getstring( xdrs, 0, s);
	} else if (xdrs->x_op == XDR_FREE) {
	    if (*s) {
		free(*s);
		*s = NULL;
		res = TRUE;
	    }
	} else res=FALSE;
    } else {
        res = xdr_wrapstring( xdrs, s);
    }
    return res;
}

bool_t xdrxml_group( XDR *xdrs, const char *s) 
{
    xdr_push_note( xdrs, s);
}

bool_t xdrxml_endgroup( XDR *xdrs)
{
    xdr_pop_note( xdrs);
}

bool_t xdrxml_getstring( XDR *xdrs, int prealloc_len, char **s) {
    /* get some bytes from " */
    struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
    xmlNodePtr cur = xdrd->cur;
    xmlDocPtr doc = xdrd->doc;
    char *attr = xdrd->attr;
    char *value, *cpos, *err;
    int c;
    int i;
    int len;

    if (!attr) attr="string";

    cur = bfs1( cur, attr);
    if (!cur) {
	printf("No node %s/%s: Using empty string\n", xdrd->path, attr);
	*s = malloc(1);
	(*s)[0]=0;
	return TRUE;
    }
    mark(cur);
    value = xmlNodeListGetString(doc, cur->children, 1);
    if (!value) return FALSE;

/*    printf("Decoding node %s value '%s'\n", attr, value); /**/

    /* get the final string size */
    for (len=0, cpos=value; *cpos; cpos++) {
        len++;
	if (cpos[0] == '=' && cpos[1] != 0 && cpos[2] != 0) cpos += 2;
    }

    if (prealloc_len > 0 && len >= prealloc_len) {
	/* not enough space preallocated in the incoming string buffer */
	fprintf(stderr, "Truncated string %s to %d characters\n", value, prealloc_len);
	len = prealloc_len-1;
    } else {
	/* allocate the string */
	*s = malloc(len+1);
    }

    /* copy from stream to string */
    cpos = value;
    for (i=0; i<len; i++) {
	c = *cpos++;
	if (c != '=') {
	    (*s)[i] = c;
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
	    (*s)[i] = c;
	}
    }
    (*s)[len]=0;
    XDRXML_DATA(xdrs)->attr = 0;
    return TRUE;
}

bool_t xdrxml_putstring( XDR *xdrs, char *s) {
    /* put some bytes to " */
    FILE *fp;
    int i;
    unsigned char c;
    int len;
    char *attr = XDRXML_DATA(xdrs)->attr;
    int ni = nchar( XDRXML_DATA(xdrs)->path, '/');

    fp = XDRXML_DATA(xdrs)->fp;
    if (!fp) fp = stdout;
    if (!attr) attr = "string";
    
    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s>", attr);
    len = strlen( s);
    for (i=0; i<len; i++) {
	c = s[ i];
	if (c >=' ' && c <='~' && c != '=' && c != '&' && c != '<') {
	    putc( c, fp);
	} else {
	    fprintf(fp, "=%02x", c);
	}
    } /* for */
    fprintf(fp, "</%s>\n", attr);

    XDRXML_DATA(xdrs)->attr = 0;
    return TRUE;
}

bool_t xdrxml_getbit( XDR *xdrs, bool_t *bit) {
    /* find a bit */
    struct xdrxml_st *xdrd = XDRXML_DATA(xdrs);
    xmlNodePtr cur = xdrd->cur;
    char *attr = xdrd->attr;

    if (!attr) attr="bit";

    /* assume bit clear */
    *bit = 0;

    cur = bfs1( cur, attr);
    if (cur) {
        /* bit set */
	*bit = 1;
	mark(cur);
    }
    return TRUE;
}

bool_t xdrxml_putbit( XDR *xdrs, bool_t bit) {
    /* put a bit to the stream */
    FILE *fp;
    char *attr = XDRXML_DATA(xdrs)->attr;
    int ni = nchar( XDRXML_DATA(xdrs)->path, '/');

    if (!bit) return TRUE;
    fp = XDRXML_DATA(xdrs)->fp;
    if (!fp) fp = stdout;
    if (!attr) attr = "bit";
    
    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s/>\n", attr);
    return TRUE;
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
	    while (ni--) xdrd->ext->x_printf( xdrs, "    ");
	    xdrd->ext->x_printf( xdrs, "<%s>\n", s); 
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
	    int ni = nchar( xdrd->path, '/');

	    while (ni--) xdrd->ext->x_printf( xdrs, "    ");
	    xdrd->ext->x_printf( xdrs, "</%s>\n", cpos); 
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
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    xmlDocPtr doc = XDRXML_DATA(__xdrs)->doc;
    char *attr = xdrd->attr;
    xmlNodePtr cur;
    char *value;
    char *err;

    if (!attr) attr="long";

    cur = bfs1( xdrd->cur, attr);
    if (!cur) {
	printf("No node %s/%s: Using value 0\n", xdrd->path, attr);
	*__lp = 0;
        xdrd->error = DR1_ENODEMISSING;
        return FALSE;
    }

    value = xmlNodeListGetString(doc, cur->children, 1);
    if (!value) return FALSE;

    *__lp = strtol( value, &err, 0);
    if (*err != 0) {
        printf("Badly formatted long at '%s'='%s'\n", attr, value);
	return FALSE;
    }
    mark(cur);
    xdrd->attr = NULL;

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
    fprintf(fp, "<%s>%ld</%s>\n", attr, *__lp, attr);
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
    XDRXML_DATA(__xdrs)->attr = 0;
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
    if (xdrd->fp) {
	fclose(xdrd->fp);
	xdrd->fp = NULL;
    }
    if (xdrd->doc) {
	xmlFreeDoc( xdrd->doc);
	xdrd->doc = NULL;
    }

}

bool_t xdrxml_getint32( XDR *__xdrs, int32_t *__ip)
{
    /* get a int from underlying stream */
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    char *attr = xdrd->attr;
    xmlNodePtr cur;
    char *value;
    char *err;

    if (!attr) attr="long";

    cur = bfs1( xdrd->cur, attr);
    if (!cur) return FALSE;

    value = xmlNodeListGetString(xdrd->doc, cur->children, 1);
    if (!value) return FALSE;

    *__ip = (int32_t)strtol( value, &err, 0);
    if (*err != 0) {
        printf("Badly formatted long at '%s'='%s'\n", attr, value);
	return FALSE;
    }
    mark(cur);

    xdrd->attr = NULL;
    return TRUE;
}

bool_t xdrxml_putint32( XDR *__xdrs, __const int32_t *__ip)
{
    /* put a int to stream */
    FILE *fp;
    struct xdrxml_st *xdrd = XDRXML_DATA(__xdrs);
    char *attr = xdrd->attr;
    int ni = nchar( xdrd->path, '/');

    if (!attr) attr = "long";
    if (xdrd->fp) fp = xdrd->fp;
    else fp = stdout;

    while (ni--) fprintf(fp, "    ");
    fprintf(fp, "<%s>0x%lx</%s>\n", attr, (long)*__ip, attr);
    xdrd->attr = NULL;
    return TRUE;
}
