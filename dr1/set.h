#ifndef __DR1SET__H
#define __DR1SET__H

/*-------------------------------------------------------------------
 * dr1Set
 *
 *   Sparse Set implementation.  Data in the Set is kept sorted so that
 *   lookup is faster, but inserts are O(n).
 */

enum { MONEYBASE=0x1000};

typedef int* dr1Set;

int dr1Set_contains( int el);
void dr1Set_union( dr1Set a, dr1Set b);
void dr1Set_intersection( dr1Set a, dr1Set b);
dr1Set dr1Set_dup( dr1Set a);
void dr1Set_add(dr1Set *a, int el);
int dr1Set_length( dr1Set a);

/*-------------------------------------------------------------------
 *
 */
