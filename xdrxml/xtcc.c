#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/param.h>

#include "nametable.h"
#include "y.tab.h"
char *fname;
FILE *emitfile;
int lineno;
extern int yydebug;
int yyparse(void);

char *strupr( char *src) {
    char *cpos;
    for (cpos = src; *cpos != 0; cpos++) {
	*cpos = toupper(*cpos);
    }
    return src;
}

char *strlwr( char *src) {
    char *cpos;
    for (cpos = src; *cpos != 0; cpos++) {
	*cpos = tolower(*cpos);
    }
    return src;
}

char *strdup2( char* lpos, char *rpos) {
    char *buf = malloc(rpos - lpos + 2);
    memcpy(buf, lpos, rpos-lpos+1);
    buf[rpos-lpos+1] = 0;
    return buf;
}
char *basename( char *path, char *ext) {
    char *lpos = rindex(path, '/');
    char *rpos = rindex(path, '.');

    if (lpos) {
        lpos++;
	if (rpos && !strcmp( rpos, ext)) {
	    rpos--;
	    return strdup2( lpos, rpos);
	} else {
	    return strdup( lpos);
	}
    } else {
	if (rpos && !strcmp( rpos, ext)) {
	    rpos--;
	    return strdup2( path, rpos);
	} else {
	    return strdup( path);
	}
    }
}

char *dirname( char *path) {
    char *rpos = rindex(path, '/');
    if (rpos) {
        rpos--;
	return strdup2(path, rpos);
    } else {
        return strdup(".");
    }
}

int emitout( char *fname) {
    emitfile = fopen( fname, "w");
    return 0;
}
int emitend( void) {
    fclose(emitfile);
    return 0;
}
int emit( int indent, char *fmt, ...) {
    va_list va;
    int i;
    va_start(va, fmt);
    for (i=0; i<indent; i++) {
	fprintf(emitfile,"    ");
    }
    vfprintf(emitfile,fmt,va);
    fprintf(emitfile,"\n");
    va_end(va);
    return 0;
}

int hentry( struct NameEntry *ne) {
    switch (ne->type) {
	case T_STRING:
	    emit(1, "char *%s;", ne->name);
	    break;
	case T_INTEGER:
	    emit(1, "int %s;", ne->name);
	    break;
	case T_BOOLEAN:
	    emit(1, "bool_t %s;", ne->name);
	    break;
	case T_STRUCT:
	    emit(1, "struct %s %s;", 
		 ne->complextype->name,
		 ne->name);
	    break;
	case T_ENUM:
	    emit(1, "enum %s %s;", 
		 ne->enumtype->name,
		 ne->name);
	    break;
	case T_POINTER:
	    emit(1, "struct %s* %s;",
		 ne->complextype->name,
		 ne->name);
	    break;
	case T_ARRAY:
	    emit(1, "struct %sArray %s;",
		 ne->complextype->name,
		 ne->name);
	    break;
	case T_POINTER_ARRAY:
	    emit(1, "struct %sPtrArray %s;",
		 ne->complextype->name,
		 ne->name);
	    break;
	default:
	    emit(0, "*** unexpected type %d", ne->type);
	    break;
    }
    return 0;
}

int hstructref( struct StructEntry *se) {
    emit(0, "struct %s;", se->name);
    emit(0, "bool_t xdr_%s( XDR* xdrs, char *node, struct %s* s);",
	    se->name,
	    se->name);
    return 0;
}

int hstruct( struct StructEntry *se) {
    int i;
    if (se->entry_size == 0) return 0;     /* blank definition */
    emit(0, "struct %s {", se->name);
    for (i=0; i<se->entry_size; i++) {
	hentry( se->entry[i]);
    }
    emit(0, "};");
    return 0;
}

int hstructptr( struct StructEntry *se) {
    if (!se->pointer) return 0;
    emit(0, "bool_t xdr_%sPtr( XDR* xdrs, char *node, struct %s** sptr);",
	    se->name,
	    se->name);
    return 0;
}

int hstructarray( struct  StructEntry *se) {
    if (!se->array) return 0;
    emit(0, "struct %sArray {", se->name);
    emit(1, "int max;   /* allocated size of the array */");
    emit(1, "int last;  /* last element currently in use */");
    emit(1, "struct %s* element;", se->name);
    emit(0, "};");
    emit(0, "bool_t xdr_%sArray( XDR* xdrs, char *node, struct %sArray* sptr);",
	    se->name,
	    se->name);
    emit(0, "int %sArray_grow( struct %sArray* sptr, int newsize);", se->name);
    return 0;
}

int hstructptrarray( struct  StructEntry *se) {
    if (!se->ptrarray) return 0;
    if (se->entry_size == 0) {
	emit(0, "struct %sPtrArray;", se->name);
    } else {
	emit(0, "struct %sPtrArray {", se->name);
	emit(1, "int max;   /* allocated size of the array */");
	emit(1, "int last;  /* last element currently in use */");
	emit(1, "struct %s** element;", se->name);
	emit(0, "};");
    }
    emit(0, "bool_t xdr_%sPtrArray( XDR* xdrs, char *node, struct %sPtrArray* sptr);",
	    se->name,
	    se->name);
    emit(0, "int %sPtrArray_grow( struct %sPtrArray* sptr, int newsize);", se->name);
    return 0;
}

int henum( struct EnumList *el) {
    int i;
    char *uctype;
    char *ucvalue;
    uctype = strupr(strdup( el->name));

    emit(0, "enum %s {", el->name);
    for (i=0; i<el->value_size; i++) {
	ucvalue = strupr(strdup( el->value[i]));

	emit(1, "%s_%s,", uctype, ucvalue );

	free(ucvalue);
    }
    emit(1, "%s_UNDEFINED", uctype);
    emit(0, "};");

    emit(0, "bool_t xdr_%s( XDR* xdrs, char *node, enum %s* e);", el->name, el->name);

    free(uctype);
    return 0;
}

int htable( struct NameTable *nt) {
    struct NameEntry *ne;
    int i;
    for (i=0; i<nt->name_size; i++) {
        ne = nt->name[i];
        if (ne->type == T_STRUCT) {
	    hstructref( ne->complextype);
	}
    }
    for (i=0; i<nt->name_size; i++) {
        ne = nt->name[i];
	switch (ne->type) {
	    case T_STRUCT:
		hstruct( ne->complextype);
		hstructptr( ne->complextype);
		hstructarray( ne->complextype);
		hstructptrarray( ne->complextype);
		break;

	    case T_ENUM:
	        henum( ne->enumtype);
		break;

	    default:
		fprintf(stderr, "Unhandled top level type %d\n", ne->type);
	} 

    }
    return 0;
}

int centry( struct NameEntry *ne) {
    switch (ne->type) {
	case T_STRING:
	    emit(1, "if (!xdrxml_wrapstring( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->name, ne->name);
	    break;
	case T_INTEGER:
	    emit(1, "if (!xdrxml_int( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->name, ne->name);
	    break;
	case T_BOOLEAN:
	    emit(1, "if (!xdrxml_bitfield( xdrs, \"%s\", s->%s, itemp, res)) return FALSE;",
		 ne->name, ne->name);
	    break;
	case T_STRUCT:
	    emit(1, "if (!xdr_%s( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->complextype->name,
		 ne->name, ne->name);
	    break;
	case T_ENUM:
	    emit(1, "if (!xdr_%s( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->enumtype->name,
		 ne->name, ne->name);
	    break;
	case T_POINTER:
	    emit(1, "if (!xdr_%sPtr( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->complextype->name,
		 ne->name, ne->name);
	    break;
	case T_ARRAY:
	    emit(1, "if (!xdr_%sArray( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->complextype->name,
		 ne->name, ne->name);
	    break;
	case T_POINTER_ARRAY:
	    emit(1, "if (!xdr_%sPtrArray( xdrs, \"%s\", &s->%s)) return FALSE;",
		 ne->complextype->name,
		 ne->name, ne->name);
	    break;
	default:
	    emit(0, "*** unexpected type %d", ne->type);
	    break;
    }
    return 0;
}

int cstruct( struct StructEntry *se) {
    int i;
    int hasbool=0;
    for (i=0; i<se->entry_size; i++) {
	if (se->entry[i]->type == T_BOOLEAN) hasbool=1;
    }
    emit(0, "bool_t xdr_%s( XDR* xdrs, char *node, struct %s* s) {",
	    se->name,
	    se->name);
    if (hasbool) {
	emit(1, "bool_t res; int itemp;");
    }
    emit(1, "xdrxml_group( xdrs, node);");
    for (i=0; i<se->entry_size; i++) {
	centry( se->entry[i]);
    }
    emit(1, "xdrxml_endgroup( xdrs);");
    emit(1, "return TRUE;");
    emit(0, "}");
    return 0;
}


int cstructptr( struct StructEntry *se) {
    if (!se->pointer) return 0;
    emit(0, "bool_t xdr_%sPtr( XDR* xdrs, char *node, struct %s** sptr) {",
	    se->name,
	    se->name);
    emit(1, "bool_t res;");
    emit(1, "if (xdrs->x_op == XDR_DECODE) {");
    emit(2, "*sptr=calloc(sizeof(struct %s),1);", se->name);
    emit(1, "}");
    emit(1, "if (*sptr != NULL) {");
    emit(2, "res = xdr_%s( xdrs, node, *sptr);");
    emit(1, "}");
    emit(1, "if ( xdrs->x_op == XDR_FREE");
    emit(3, "|| (xdrs->x_op == XDR_DECODE && !res) )");
    emit(1, "{");
    emit(2, "free(*sptr);");
    emit(2, "*sptr = NULL;");
    emit(1, "}");
    emit(1, "return TRUE;");
    emit(0, "}");
    return 0;
}

int cstructarray( struct StructEntry *se) {
    if (!se->array) return 0;
    emit(0, "bool_t xdr_%sArray( XDR* xdrs, char *node, struct %sArray* sptr) {",
	    se->name,
	    se->name);
    emit(1, "int i;");
    emit(1, "if (xdrs->x_op == XDR_DECODE) {");
    emit(2, "if (!xdrxml_arraysize( xdrs, node, &sptr->max)) return FALSE;");
    emit(2, "sptr->last = sptr->max;");
    emit(2, "sptr->element=calloc(sizeof(struct %s),sptr->max);",se->name);
    emit(1, "}");
    emit(1, "for (i=0; i<sptr->last; i++) {");
    emit(2, "if (!xdr_%s( xdrs, node, &sptr->element[i])) return FALSE;",se->name);
    emit(1, "} /* for */");
    emit(1, "if ( xdrs->x_op == XDR_FREE) {");
    emit(2, "free(sptr->element);");
    emit(2, "sptr->element = NULL;");
    emit(1, "}");
    emit(1, "return TRUE;");
    emit(0, "}");
    emit(0, "int %sArray_grow( struct %sArray* sptr, int newsize) {", se->name, se->name);
    emit(1, "int qsize = (((newsize-1) / QUANTUM)+1) * QUANTUM;");
    emit(1, "if (qsize < sptr->max) {");
    emit(2, "sptr->element = realloc(sptr->element, sizeof(struct %s) * qsize);", se->name);
    emit(1, "}");
    emit(1, "return qsize;");
    emit(0, "}");
    return 0;
}

int cstructptrarray( struct StructEntry *se) {
    if (!se->ptrarray) return 0;
    emit(0, "bool_t xdr_%sPtrArray( XDR* xdrs, char *node, struct %sPtrArray* sptr) {",
	    se->name,
	    se->name);
    emit(1, "int i;");
    emit(1, "if (xdrs->x_op == XDR_DECODE) {");
    emit(2, "if (!xdrxml_arraysize( xdrs, node, &sptr->max)) return FALSE;");
    emit(2, "sptr->last = sptr->max;");
    emit(2, "sptr->element=calloc(sizeof(struct %s*),sptr->max);",se->name);
    emit(1, "}");
    emit(1, "for (i=0; i<sptr->last; i++) {");
    emit(2, "if (!xdr_%sPtr( xdrs, node, &sptr->element[i])) return FALSE;",se->name);
    emit(1, "} /* for */");
    emit(1, "if ( xdrs->x_op == XDR_FREE) {");
    emit(2, "free(sptr->element);");
    emit(2, "sptr->element = NULL;");
    emit(1, "}");
    emit(1, "return TRUE;");
    emit(0, "}");
    emit(0, "int %sPtrArray_grow( struct %sPtrArray* sptr, int newsize) {", se->name, se->name);
    emit(1, "int qsize = (((newsize-1) / QUANTUM)+1) * QUANTUM;");
    emit(1, "if (qsize < sptr->max) {");
    emit(2, "sptr->element = realloc(sptr->element, sizeof(struct %s*) * qsize);", se->name);
    emit(1, "}");
    emit(1, "return qsize;");
    emit(0, "}");
    return 0;
}

int cenum( struct EnumList *el) {
    int i;
    char *uctype;
    char *ucvalue;
    char *lcvalue;
    uctype = strupr(strdup( el->name));

    emit(0, "bool_t xdr_%s( XDR* xdrs, char *node, enum %s* e)", el->name, el->name);
    emit(0, "{");
    emit(1, "bool_t xval;");
    emit(1, "xdrxml_group( xdrs, node);");
    emit(1, "if (xdrs->x_op == XDR_DECODE) {");
    for (i=0; i<el->value_size; i++) {
        lcvalue = strlwr(strdup( el->value[i]));
	ucvalue = strupr(strdup( el->value[i]));

	emit(2, "%sif (xdrxml_bool( xdrs, \"%s\", &xval) && xval==1) {", 
		i==0?"":"} else ", lcvalue);
	emit(3, "*e=%s_%s;", uctype, ucvalue);

	free(lcvalue); free(ucvalue);
    }
    emit(2, "} else {");
    emit(3, "return FALSE;");
    emit(2, "}");
    emit(1, "} else {");
    emit(2, "xval=TRUE;");
    emit(2, "switch (*e) {");
    
    for (i=0; i<el->value_size; i++) {
        lcvalue = strlwr(strdup( el->value[i]));
	ucvalue = strupr(strdup( el->value[i]));

        emit(3, "case %s_%s:", uctype, ucvalue);
	emit(4, "if (!xdrxml_bool( xdrs, \"%s\", &xval)) return FALSE;", lcvalue);
	emit(4, "break;");

	free(lcvalue); free(ucvalue);
    }
    emit(3, "default:");
    emit(4, "return FALSE;");
    emit(2, "} /* switch */");
    emit(1, "} /* if */");
    emit(1, "xdrxml_endgroup( xdrs);");
    emit(1, "return TRUE;");
    emit(0, "}");

    free(uctype);
    return 0;
}

int ctable( struct NameTable *nt) {
    int i;
    struct StructEntry *se;
    struct NameEntry *ne;
    for (i=0; i<nt->name_size; i++) {
        ne = nt->name[i];
	switch (ne->type) {
	    case T_STRUCT:
	        se=ne->complextype;
		if (se->entry_size) {
		    /* not a blank definition */
		    cstruct( se);
		    cstructptr( se);
		    cstructarray( se);
		    cstructptrarray( se);
		}
		break;
	    case T_ENUM:
	        cenum( ne->enumtype);
		break;

	    default:
	    	fprintf(stderr, "! Unhandled top level type %d.\n", ne->type);
		break;
	}
    }
    return 0;
}

int main( int argc, char **argv) {
    FILE *in;
    char *fnameb;
    char *FNAMEB;
    char ofile[MAXPATHLEN];
/*    yydebug=1; /**/
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
	exit(1);
    }
    fname = argv[1];
    fnameb = basename(fname, ".x");
    FNAMEB = strupr( strdup(fnameb));
    in = freopen(fname, "r", stdin);
    if (in == NULL) {
	fprintf(stderr, "Unable to open file %s\n", fname);
	exit(1);
    }
    yyparse();
    fclose(in);
#if 0
    NameTable_dump(&globalNameTable);
#endif
    snprintf(ofile, MAXPATHLEN, "%s/%s.h", dirname(fname), fnameb);
    fprintf(stderr, "Writing %s\n", ofile);
    emitout( ofile);
    emit(0, "#ifndef __%s__H", FNAMEB);
    emit(0, "#define __%s__H", FNAMEB);
    emit(0, "#include \"rpc/xdr.h\"");
    htable(&globalNameTable);
    emit(0, "#endif");
    emitend();

    snprintf(ofile, MAXPATHLEN, "%s/%s.c", dirname(fname), fnameb);
    fprintf(stderr, "Writing %s\n", ofile);
    emitout( ofile);
    emit(0, "/* %s.c */", fnameb);
    emit(0, "#include \"%s.h\"", fnameb);
    emit(0, "#include \"xdrxml.h\"");
    emit(0, "#define QUANTUM 10");
    ctable(&globalNameTable);
    emitend();

    return 0;
}

int yyerror( char *s) {
    return fprintf(stderr, "%s+%d Error: %s\n", fname, lineno, s);
}
