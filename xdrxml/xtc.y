%{
#include <stdio.h>
#include "nametable.h"
%}
%union {
    struct NameEntry *ne;
    struct StructEntry *se;
    struct NameTable *nt;
    struct EnumList *el;
    enum BasicType type;
    char *s;
}

%token STRUCT ARRAY
%token <s> VARNAME STRNAME 
%token <type> BOOL INT STRING 

%type <s> _stype
%type <se> _struct_decl _struct_defn _strents
%type <type> _type
%type <ne> _strent

%start _file

%%

_file :
	_component
	| _file _component

_component :
	_struct_decl
	| _struct_defn
	| _enum

_struct_decl :
	STRUCT STRNAME ';'
	{
	    struct StructEntry *se;
	    se = StructEntry_create();
	    se->name = $2;
	    NameTable_add( &globalNameTable, se);
	    $$ = se;
	}

_struct_defn : 
	STRUCT STRNAME '{' _strents '}'
	{
	    struct StructEntry *tmp;
	    
	    tmp = NameTable_find( &globalNameTable, $2);
	    if (tmp) {
		/* blank declaration now completed */
	        if (tmp->entry_size != 0) {
		    fprintf(stderr, "Duplicate structure definition '%s'\n",
		        $2);
		    exit( 1);
		}
		/* reparent struct entries */
	        *tmp = *$4;
		/* shallow free */
		free( $4);  
	    } else {
	        tmp = $4;
		$4->name = $2;
		NameTable_add( &globalNameTable, $4);
	    }
	    $$ = tmp;
	}

_strents :
	_strent
	{
	    struct StructEntry *se;
	    se = StructEntry_create();
	    StructEntry_add( se, $1);
	    $$ = se;
	}
	| _strents _strent
	{
	    StructEntry_add( $1, $2);
	    $$ = $1;
	}

_strent :
	_type VARNAME ';'
	{
	    struct NameEntry *ne;
	    ne = NameEntry_create( $1, $2, "");
	    free($2);
	    $$ = ne;
	}
	| _stype VARNAME ';'
	{
	    struct StructEntry *se;
	    struct NameEntry *ne;
	    se = NameTable_find( &globalNameTable, $1);
	    if (se == NULL) {
		fprintf( stderr, "Undefined structure '%s'\n", $1);
		exit(1);
	    }
	    if (!se->entry_size) {
		fprintf( stderr, "! Incomplete structure '%s'\n", $1);
	    }
	    ne = NameEntry_createComplex( se, $2);
	    free($2);
	    $$ = ne;
	}
	| _stype '*' VARNAME ';'
	{
	    struct StructEntry *se;
	    struct NameEntry *ne;
	    se = NameTable_find( &globalNameTable, $1);
	    if (se == NULL) {
		fprintf( stderr, "Undefined structure '%s'\n", $1);
		exit(1);
	    }
	    ne = NameEntry_createPointer( se, $3);
	    free($3);
	    $$ = ne;
	}
	| ARRAY '<' _stype '>' VARNAME ';'
	{
	    struct StructEntry *se;
	    struct NameEntry *ne;
	    se = NameTable_find( &globalNameTable, $3);
	    if (se == NULL) {
		fprintf( stderr, "Undefined structure '%s'\n", $3);
		exit(1);
	    }
	    ne = NameEntry_createArray( se, $5);
	    free($5);
	    $$ = ne;
	}
	| ARRAY '<' _stype '*' '>' VARNAME ';'
	{
	    struct StructEntry *se;
	    struct NameEntry *ne;
	    se = NameTable_find( &globalNameTable, $3);
	    if (se == NULL) {
		fprintf( stderr, "Undefined structure '%s'\n", $3);
		exit(1);
	    }
	    ne = NameEntry_createPtrArray( se, $6);
	    free($6);
	    $$ = ne;
	}
	| _etype VARNAME ';'
	{
	    struct NameEntry *ne;
	    ne = NameEntry_createEnum( $1, $2); 
	    $$ = ne;
	}

_type :
	BOOL		{ $$ = T_BOOLEAN; }
	| INT		{ $$ = T_INTEGER; }
	| STRING	{ $$ = T_STRING; }

_stype :
	STRUCT STRNAME	
	{
	    $$ = $2;
	}

_etype :
	_enumref
	| _enum

_enumref :
	ENUM STRNAME
	{
	    struct EnumList *el = NameTable_find( &globalNameSpace, $2);
	    $$ = el;
	}

_enum :
	ENUM STRNAME '{' _enumlist '}'
	{
	    $4->name = $2;
	    $$ = $4;
	}

_enumlist :
	_enumitem
	{
	    struct EnumList *el = EnumList_create();
	    EnumList_add( el, $1);
	    $$ = el;
	}
	| _enumlist _enumitem
	{
	    EnumList_add( $1, $2);
	    $$ = $1;
	}

_enumitem : VARNAME
	{
	    $$ = $1;
	}
	
%%
