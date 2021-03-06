#include "string.h"

enum BasicType {
    T_BOOLEAN,
    T_INTEGER,
    T_STRING,
    T_ARRAY,
    T_POINTER,
    T_POINTER_ARRAY,
    T_INTEGER_ARRAY,
    T_STRING_ARRAY,
    T_STRUCT,
    T_ENUM
};

struct StructEntry;

struct NameEntry {
    enum BasicType type;
    struct StructEntry* complextype;
    struct EnumList* enumtype;
    char* name;
    char* initializer;
};

struct StructEntry {
    char *name;
    int pointer;		/* add pointer functions */
    int array;			/* add array type and functions */
    int ptrarray;		/* add array of pointers function */
    int entry_size;
    struct NameEntry **entry;
};

struct NameTable {
    int name_size;
    struct NameEntry **name;
};

struct EnumList {
    char *name;
    int value_size;
    char **value;
    char **initializer;
};

extern struct NameTable globalNameTable;
	
void NameTable_add( struct NameTable* nt, struct NameEntry* ne); 
void NameTable_addStruct( struct NameTable* nt, struct StructEntry* se); 
void NameTable_addEnum( struct NameTable* nt, struct EnumList* el); 

void NameTable_dump( struct NameTable* nt);

struct NameEntry* NameTable_find( struct NameTable* nt, char *name); 
struct StructEntry* NameTable_findStruct( struct NameTable* nt, char *name); 
struct EnumList* NameTable_findEnum( struct NameTable* nt, char *name); 

struct StructEntry* StructEntry_create( void);
void StructEntry_add( struct StructEntry *se, struct NameEntry *ne); 

struct EnumList* EnumList_create( void);
void EnumList_add( struct EnumList *el, char *name); 

struct NameEntry* NameEntry_create( 
	enum BasicType type,
	char* name,
	char* initializer);

struct NameEntry* NameEntry_createComplex( 
        struct StructEntry* type,
	char* name);

struct NameEntry* NameEntry_createPointer( 
        struct StructEntry* type,
	char* name);

struct NameEntry* NameEntry_createArray( 
        struct StructEntry* type,
	char* name);

struct NameEntry* NameEntry_createPtrArray( 
        struct StructEntry* type,
	char* name);

struct NameEntry* NameEntry_createEnum( struct EnumList *el, char *name);
