#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nametable.h"

struct NameTable globalNameTable;

void
NameTable_add( struct NameTable* nt, struct NameEntry* ne) 
{
    int i;
    i = nt->name_size++;
    nt->name = realloc( nt->name, sizeof(struct NameEntry*) * nt->name_size);
    nt->name[i] = ne;
}

void
NameTable_addStruct( struct NameTable* nt, struct StructEntry* se) 
{
    struct NameEntry *ne;
    ne = NameEntry_createComplex( se, "");
    NameTable_add( nt, ne);
}

void
NameTable_addEnum( struct NameTable* nt, struct EnumList* el) 
{
    struct NameEntry *ne;
    ne = NameEntry_createEnum( el, "");
    NameTable_add( nt, ne);
}

struct NameEntry* NameTable_find( struct NameTable* nt, char *name) {
    int i;
    struct NameEntry *ne;
    for (i=0; i<nt->name_size; i++) {
        ne = nt->name[i];
        switch (ne->type) {
	    case T_STRUCT:
	        if (strcmp(ne->complextype->name, name)==0) {
		    return ne;
		}
		break;
	    case T_ENUM:
	        if (strcmp(ne->enumtype->name, name)==0) {
		    return ne;
		}
		break;
	    default:
		break;
	}
    }
    return NULL;
}

struct StructEntry* NameTable_findStruct( struct NameTable* nt, char *name) {
    struct NameEntry *ne = NameTable_find( nt, name);
    if (!ne) return NULL;
    if (ne->type != T_STRUCT) {
	fprintf(stderr, "Expected struct type %s\n", name);
	exit(1);
    }
    return ne->complextype;
}

struct EnumList* NameTable_findEnum( struct NameTable* nt, char *name) {
    struct NameEntry *ne = NameTable_find( nt, name);
    if (!ne) return NULL;
    if (ne->type != T_ENUM) {
	fprintf(stderr, "Expected enum type %s\n", name);
	exit(1);
    }
    return ne->enumtype;
}

void NameTable_dump( struct NameTable *nt) {
    int i,j;
    struct StructEntry *se;
    struct NameEntry *ne;
    struct EnumList *el;
    for (i=0; i<nt->name_size; i++) {
	printf("nt:name[%d]\n",i);
	printf("nt:name->type=%d\n",nt->name[i]->type);
	if (nt->name[i]->type == T_STRUCT) {
	    se = nt->name[i]->complextype;
	    printf("se:name=%s\n",se->name);
	    printf("se:pointer=%d\n",se->pointer);
	    for (j=0; j<se->entry_size; j++) {
		printf("se:entry[%d]\n",j);
		ne = se->entry[j];
		printf("ne:type=%d\n", ne->type);
		if (ne->type == T_STRUCT || ne->type == T_POINTER) {
		    printf("ne:complextype=%s\n", ne->complextype->name);
		}
		printf("ne:name=%s\n", ne->name);
	    }
	} else if (nt->name[i]->type == T_ENUM) {
	    el = nt->name[i]->enumtype;
	    printf("el:name=%s\n", el->name);
	    for (j=0; j<el->value_size; j++) {
		printf("el->value[%d] = %s\n", j, el->value[j]);
	    }
	}
    }
}

struct StructEntry* StructEntry_create( void) {
    struct StructEntry *se = calloc( sizeof(struct StructEntry), 1);
    return se; 
}

void
StructEntry_add( struct StructEntry *se, struct NameEntry *ne) 
{
    int i;
    i = se->entry_size++;
    se->entry = realloc( se->entry, sizeof(struct StructEntry*) * se->entry_size);
    se->entry[i] = ne;
}

struct EnumList* EnumList_create( void) {
    struct EnumList *se = calloc( sizeof(struct EnumList), 1);
    return se; 
}

void
EnumList_add( struct EnumList *el, char *name) 
{
    int i;
    i = el->value_size++;
    el->value = realloc( el->value, sizeof(char*) * el->value_size);
    el->value[i] = name;
}

struct NameEntry* NameEntry_create( 
	enum BasicType type,
	char* name,
	char* initializer)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = type;
    ne->name = strdup(name);
    ne->initializer = strdup(initializer);
    return ne;
}

struct NameEntry* NameEntry_createComplex( 
        struct StructEntry* type,
	char* name)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = T_STRUCT;
    ne->complextype = type;
    ne->name = strdup(name);
    return ne;
}

struct NameEntry* NameEntry_createPointer( 
        struct StructEntry* type,
	char* name)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = T_POINTER;
    ne->complextype = type;
    ne->name = strdup(name);
    type->pointer=1;
    return ne;
}

struct NameEntry* NameEntry_createArray( 
        struct StructEntry* type,
	char* name)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = T_ARRAY;
    ne->complextype = type;
    ne->name = strdup(name);
    type->array=1;
    return ne;
}

struct NameEntry* NameEntry_createPtrArray( 
        struct StructEntry* type,
	char* name)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = T_POINTER_ARRAY;
    ne->complextype = type;
    ne->name = strdup(name);
    type->ptrarray=1;
    type->pointer=1;
    return ne;
}

struct NameEntry* NameEntry_createEnum( 
	struct EnumList *type,
	char *name)
{
    struct NameEntry *ne;
    ne = calloc( sizeof( struct NameEntry),1);
    ne->type = T_ENUM;
    ne->enumtype = type;
    ne->name = strdup(name);
    return ne;
}
