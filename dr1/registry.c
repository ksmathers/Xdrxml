#include "registry.h"

/*-------------------------------------------------------------------
 * dr1Registry
 *
 *    This structure maps between code values and object classes.  
 *    For example the Item class is a polymorphic class with virtual
 *    methods, so to locate those methods you can use the item 
 *    type and the item registry to locate the correct method for
 *    an object.
 */

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

void *dr1Registry_lookup( dr1Registry *r, int code) {
    int i;
    for ( i=0; r->entries[i].code != -1; i++) {
        if (r->entries[i].code == code) {
	    return r->entries[i].value;
	}
    }
    return 0;
}

