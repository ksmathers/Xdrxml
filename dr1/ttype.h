#ifndef __DR1TTYPE__H
#define __DR1TTYPE__H

#ifndef _RPC_XDR_H
#   include <rpc/xdr.h>
#endif

/*-------------------------------------------------------------------
 * dr1
 *
 *    Random treasure generation 
 */

typedef struct {
    char ttype;

    int cp_chance;
    dr1Dice cp;
    int sp_chance;
    dr1Dice sp;
    int ep_chance;
    dr1Dice ep;
    int gp_chance;
    dr1Dice gp;
    int pp_chance;
    dr1Dice pp;

    int gems_chance;
    dr1Dice gems;

    int jewelry_chance;
    dr1Dice jewelry;
} dr1TType;

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
	   /* pp */  0, ""
	   /* gems    */ 30, "d8"
	   /* jewelry */ 20, "d4"
    },
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


#endif /* __DR1__H */
