#include <rpc/xdr.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpointer.h>
#include "strbuf.h"

enum {
    DR1_ENOERROR,
    DR1_ENODEMISSING
};

/*********************************************************************
*
*  xdrxml.h
*
*  Together with properly written xdr functions, this library allows
*  you to create an xml dump of an xdr data structure.  If using
*  rpcgen you will have to by hand add the appropriate annotation
*  calls that describe the data that is being dumped.  Some output
*  is a bit bizarre due to the simplicity of the XDR serialization
*  scheme (xdr_wrapstring dumps the length, then the string, then
*  a terminator for example) whereas if there were a callback for
*  writing a string to the stream directly we could catch those
*  calls and write just the string itself.
*
*  xdr_attr() - annotates a simple (leaf) type
*  xdr_push_note() - adds a type to the type stack
*  xdr_pop_note() - removes a type from the type stack
*
*  XDR xdrxml - this global xdr stream does the dump to stdout
* 
*/

struct xdr_ops_ext {
	bool_t (*x_getbit)( XDR *xdrs, bool_t *bit);
	bool_t (*x_putbit)( XDR *xdrs, bool_t bit);
	bool_t (*x_getstring)( XDR *xdrs, int prealloc_len, char **str);
	bool_t (*x_putstring)( XDR *xdrs, char *str);
        int (*x_printf)( XDR *xdrs, char *fmt, ...);
    };

struct xdrxml_st {
        char path[1024];
	char *attr;
	int error;

	/* file output */
	FILE *fp;		/* output stream */

	/* file input */
	xmlDocPtr doc;		/* input stream */
	xmlNodePtr cur;		/* input cursor */
	xmlNodePtr last;	/* last node used */

        /* input/output to string buffer */
	dr1StringBuffer *sb;

	/* extended ops */
	struct xdr_ops_ext *ext;

    };

enum { XDR_ANNOTATE = 0x1 };

bool_t xdr_push_note( XDR *xdrs, const char *ann);
bool_t xdr_pop_note( XDR *xdrs);
void xdr_attr( XDR *xdrs, const char *ann);

/*
 * Utility functions
 */

#define mark(n) xdrxml_mark(n)
void xdrxml_mark( xmlNodePtr node);

#define bfs1(n,nm) xdrxml_bfs1(n,nm)
xmlNodePtr xdrxml_bfs1( xmlNodePtr node, const char *name); 

#define nchar(p,c) xdrxml_nchar(p,c)
int xdrxml_nchar( char *path, int c); 


/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */

extern XDR xdrxml, xdrxmlsb;

int xdrxml_error( XDR* xdrs);
void xdrxml_clearerr( XDR* xdrs);

int xdr_xml_create( XDR* xdrs, char *fname, enum xdr_op xop);
    /* open an XML xdr stream */

bool_t xdrxml_bool( XDR *xdrs, int *bool) ;
    /* read/write boolean as node (instead of int) */

bool_t xdrxml_wrapstring( XDR *xdrs, char **s) ;
    /* read/write strings as strings (instead of int+bytes+bytes) */

bool_t xdrxml_getlong( XDR *__xdrs, long *__lp);
    /* get a long from underlying stream */

bool_t xdrxml_putlong( XDR *__xdrs, __const long *__lp);
    /* put a long to " */

bool_t xdrxml_getbytes( XDR *__xdrs, caddr_t __addr,
			     u_int __len);
    /* get some bytes from " */

bool_t xdrxml_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len);
    /* put some bytes to " */

u_int xdrxml_getpostn( __const XDR *__xdrs);
    /* returns bytes off from beginning */

bool_t xdrxml_setpostn( XDR *__xdrs, u_int pos);
    /* lets you reposition the stream */

int32_t *xdrxml_inline( XDR *__xdrs, int len);
    /* buf quick ptr to buffered data */

void xdrxml_destroy( XDR *__xdrs);
    /* free privates of this xdr_stream */

bool_t xdrxml_getint32( XDR *__xdrs, int32_t *__ip);
    /* get a int from underlying stream */

bool_t xdrxml_putint32( XDR *__xdrs, __const int32_t *__ip);
    /* put a int to " */

/* Extended functions */

int xdrxml_printf( XDR *xdrs, char *fmt, ...);

bool_t xdrxml_getstring( XDR *xdrs, int prealloc_len, char **s); 
    /* get a string from the underlying stream */

bool_t xdrxml_putstring( XDR *xdrs, char *s); 
    /* put a string to the underlying stream */

bool_t xdrxml_getbit( XDR *xdrs, bool_t *bit); 
    /* get a bit from the underlying stream */

bool_t xdrxml_putbit( XDR *xdrs, bool_t bit); 
    /* put a bit to the underlying stream */

/* String Buffer functions */
int xdr_xml_sb_create( XDR* xdrs, char *buf, enum xdr_op xop);
    /* open an XML xdr stream */

int xdrxmlsb_reset( XDR* xdrs);
char* xdrxmlsb_getbuf( XDR* xdrs);

int xdrxmlsb_printf( XDR *xdrs, char *fmt, ...);

bool_t xdrxmlsb_putstring( XDR *xdrs, char *s);

bool_t xdrxmlsb_putbit( XDR *xdrs, bool_t bit);

bool_t xdrxmlsb_putlong( XDR *__xdrs, __const long *__lp);

bool_t xdrxmlsb_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len);

bool_t xdrxmlsb_putint32( XDR *__xdrs, __const int32_t *__ip);
