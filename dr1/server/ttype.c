#include <assert.h>
#include <string.h>

#include "ttype.h"

/*-------------------------------------------------------------------
 * dr1
 *
 *    Random treasure generation 
 */

dr1TType dr1treasure[] = {
    { 'A', /* cp */ 25, "d6*1000", 
	   /* sp */ 30, "d6*1000", 
	   /* ep */ 35, "d6*1000", 
	   /* gp */ 40, "d10*1000", 
	   /* pp */ 25, "d4*100", 
	   /* gems    */ 60, "4d10", 
	   /* jewelry */ 50, "3d10"
    },

    { 'B', /* cp */ 50, "d8*1000", 
	   /* sp */ 25, "d6*1000", 
	   /* ep */ 25, "d4*1000", 
	   /* gp */ 25, "d3*1000", 
	   /* pp */  0, "",
	   /* gems    */ 30, "d8",
	   /* jewelry */ 20, "d4"
    },
    
    { 'J', /* cp */100, "3d8",
	   /* sp */  0, "",
	   /* ep */  0, "",
	   /* gp */  0, "",
	   /* pp */  0, "",
	   /* gems    */ 30, "d8",
	   /* jewelry */ 20, "d4"
    }

};


/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

dr1TType* dr1TType_find( char ttype) {
    int i;
    for (i=0; i< sizeof(dr1treasure)/sizeof(dr1treasure[0]); i++) {
	if ( ttype == dr1treasure[i].ttype ) {
	    return &dr1treasure[i];
	}
    }
    return NULL;
}


/*-------------------------------------------------------------------
 * dr1
 *
 *    The method ...
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */

void dr1TType_collect( char* treasures, dr1Money *m, int *gems, int *jewelry) {
    dr1TType *t;
    char *ttype;
    int dres;

    bzero( m, sizeof(*m));
    *gems = 0;
    *jewelry = 0;

    for (ttype = treasures; *ttype != 0; ttype++) {
	t = dr1TType_find( *ttype);
	assert( t);

	/* copper */
	dres = dr1Dice_roll("d100");
	if (dres < t->cp_chance) {
	    m->cp += dr1Dice_roll( t->cp);
	}
	
	/* silver */
	dres = dr1Dice_roll("d100");
	if (dres < t->sp_chance) {
	    m->sp += dr1Dice_roll( t->sp);
	}

	/* electrum */
	dres = dr1Dice_roll("d100");
	if (dres < t->ep_chance) {
	    m->ep += dr1Dice_roll( t->ep);
	}
	
	/* gold */
	dres = dr1Dice_roll("d100");
	if (dres < t->gp_chance) {
	    m->gp += dr1Dice_roll( t->gp);
	}

	/* platinum */
	dres = dr1Dice_roll("d100");
	if (dres < t->pp_chance) {
	    m->pp += dr1Dice_roll( t->pp);
	}

	/* gems */
	dres = dr1Dice_roll("d100");
	if (dres < t->gems_chance) {
	    *gems += dr1Dice_roll( t->gems);
	}

	/* jewelry */
	dres = dr1Dice_roll("d100");
	if (dres < t->jewelry_chance) {
	    *jewelry += dr1Dice_roll( t->jewelry);
	}
    } /* for */
} /* collect() */

