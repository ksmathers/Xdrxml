#include <rpc/xdr.h>

/*********************************************************************
*
*  xdrasc.h
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
*  XDR xdrasc - this global xdr stream does the dump to stdout
* 
*/

enum { XDR_ANNOTATE = 0x1 };

bool_t xdr_push_note( XDR *xdrs, const char *ann, ...);
bool_t xdr_pop_note( XDR *xdrs);
void xdr_attr( XDR *xdrs, const char *ann, ...);

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */

extern XDR xdrasc;

bool_t xdrasc_getlong( XDR *__xdrs, long *__lp);
    /* get a long from underlying stream */

bool_t xdrasc_putlong( XDR *__xdrs, __const long *__lp);
    /* put a long to " */

bool_t xdrasc_getbytes( XDR *__xdrs, caddr_t __addr,
			     u_int __len);
    /* get some bytes from " */

bool_t xdrasc_putbytes( XDR *__xdrs, __const char *__addr,
			     u_int __len);
    /* put some bytes to " */

u_int xdrasc_getpostn( __const XDR *__xdrs);
    /* returns bytes off from beginning */

bool_t xdrasc_setpostn( XDR *__xdrs, u_int pos);
    /* lets you reposition the stream */

int32_t *xdrasc_inline( XDR *__xdrs, int len);
    /* buf quick ptr to buffered data */

void xdrasc_destroy( XDR *__xdrs);
    /* free privates of this xdr_stream */

bool_t xdrasc_getint32( XDR *__xdrs, int32_t *__ip);
    /* get a int from underlying stream */

bool_t xdrasc_putint32( XDR *__xdrs, __const int32_t *__ip);
    /* put a int to " */

