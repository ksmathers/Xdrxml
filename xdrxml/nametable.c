#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nametable.h"

struct NameTable globalNameTable;

void
NameTable_add( struct NameTable* nt, struct StructEntry* se) 
{
    int i;
    i = nt->name_size++;
    nt->name = realloc( nt->name, sizeof(struct StructEntry*) * nt->name_size);
    nt->name[i] = se;
}

struct StructEntry* NameTable_find( struct NameTable* nt, char *name) {
    int i;
    for (i=0; i<nt->name_size; i++) {
	if (strcmp(nt->name[i]->name, name)==0) {
	    return nt->name[i];
	}
    }
    return NULL;
}

void NameTable_dump( struct NameTable *nt) {
    int i,j;
    struct StructEntry *se;
    struct NameEntry *ne;
    for (i=0; i<nt->name_size; i++) {
	printf("nt:name[%d]\n",i);
	se = nt->name[i];
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
