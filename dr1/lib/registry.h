#ifndef __DR1REGISTRY__H
#define __DR1REGISTRY__H

/*-------------------------------------------------------------------
 * dr1Registry
 *
 *    This structure maps between code values and object classes.  
 *    For example the Item class is a polymorphic class with virtual
 *    methods, so to locate those methods you can use the item 
 *    type and the item registry to locate the correct method for
 *    an object.
 */

typedef int (*dr1Registry_compare_fnp)(void *a, void *b);

typedef struct {
    int code;
    void *value;
} dr1RegistryEntry;

typedef struct {
    dr1RegistryEntry *entries;
} dr1Registry;

/*-------------------------------------------------------------------
 * dr1Registry_lookup
 *
 *    The method dr1Registry_lookup returns the class pointed to
 *    by a class type code.
 *
 *  PARAMETERS:
 *    r     The registry to search
 *    code  The serial id for the class
 *
 *  RETURNS:
 *    Pointer to the class
 *
 */

void *dr1Registry_lookup( dr1Registry *r, int code);

/*-------------------------------------------------------------------
 * dr1Registry_findByValue
 *
 *    The method dr1Registry_findByValue returns the code for a 
 *    class.
 *
 *  PARAMETERS:
 *    r     The registry to search
 *    class The class pointer
 *    cmp   A callback function for comparing the values in the registry.
 *
 *  RETURNS:
 *    code
 */

int dr1Registry_findByValue( dr1Registry *r, void *value, 
			     dr1Registry_compare_fnp cmp);

#endif /* __DR1REGISTRY__H */
