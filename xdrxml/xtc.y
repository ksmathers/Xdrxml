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

%type <s> _stype _enumitem
%type <se> _strents
%type <type> _type
%type <ne> _strent
%type <el> _etype _enum _enumref _enumlist

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
	    NameTable_addStruct( &globalNameTable, se);
	}

_struct_defn : 
	STRUCT STRNAME '{' _strents '}'
	{
	    struct StructEntry *se;
	    
	    se = NameTable_findStruct( &globalNameTable, $2);
	    if (se) {
		/* blank declaration now completed */
		if (se->entry_size != 0) {
		    fprintf(stderr, "Duplicate structure definition '%s'\n",
			$2);
		    exit( 1);
		}
		/* reparent struct entries */
		*se = *$4;
		/* shallow free */
		free( $4);  
	    } else {
		$4->name = $2;

		NameTable_addStruct( &globalNameTable, $4);
	    }
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
	    se = NameTable_findStruct( &globalNameTable, $1);
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
	    se = NameTable_findStruct( &globalNameTable, $1);
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
	    se = NameTable_findStruct( &globalNameTable, $3);
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
	    se = NameTable_findStruct( &globalNameTable, $3);
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
	{
	    $$ = $1;
	}

_enumref :
	ENUM STRNAME
	{
	    struct EnumList *el = NameTable_findEnum( &globalNameTable, $2);
	    if (el == NULL) {
		fprintf(stderr, "Undefined enum '%s'\n", $2);
		exit(1);
	    }
	    $$ = el;
	}

_enum :
	ENUM STRNAME '{' _enumlist '}'
	{
	    struct NameEntry *ne;
	    ne = NameTable_find( &globalNameTable, $2);
	    if (ne) {
		fprintf(stderr, "Duplicate enum definition '%s'\n", $2);
		exit(1);
	    } 
	    $4->name = $2;
	    NameTable_addEnum( &globalNameTable, $4);

	    $$ = $4;
	}

_enumlist :
	_enumitem
	{
	    struct EnumList *el = EnumList_create();
	    EnumList_add( el, $1);
	    $$ = el;
	}
	| _enumlist ',' _enumitem
	{
	    EnumList_add( $1, $3);
	    $$ = $1;
	}

_enumitem : VARNAME
	{
	    $$ = $1;
	}
	
%%
