#ifndef __DR1UTIL__H
#define __DR1UTIL__H

#ifndef FALSE
#   define TRUE 1
#   define FALSE 0
#endif

/*-------------------------------------------------------------------
 * dr1_los
 *
 *    The method ...
 *
 *  PARAMETERS:
 *    y1, x1:  Player position
 *    y2, x2:  Position to check
 *    map:     Map to read through
 *
 *  RETURNS:
 *    TRUE  in sight
 *    FALSE occluded
 *
 *  SIDE EFFECTS:
 *
 * A simple, fast, integer-based line-of-sight algorithm.  By Joseph Hall,
 * 4116 Brewster Drive, Raleigh NC 27606.  Email to jnh@ecemwl.ncsu.edu.
 *
 * This function returns TRUE if a "line of sight" can be traced from the
 * center of the grid (x1,y1) to the center of the grid (x2,y2), with all
 * of the grids along this path (except for the endpoints) being non-wall
 * grids.  Actually, the "chess knight move" situation is handled by some
 * special case code which allows the grid diagonally next to the player
 * to be obstructed, because this yields better gameplay semantics.  This
 * algorithm is totally reflexive, except for "knight move" situations.
 *
 * Because this function uses (short) ints for all calculations, overflow
 * may occur if dx and dy exceed 90.
 *
 * Once all the degenerate cases are eliminated, we determine the "slope"
 * ("m"), and we use special "fixed point" mathematics in which we use a
 * special "fractional component" for one of the two location components
 * ("qy" or "qx"), which, along with the slope itself, are "scaled" by a
 * scale factor equal to "abs(dy*dx*2)" to keep the math simple.  Then we
 * simply travel from start to finish along the longer axis, starting at
 * the border between the first and second tiles (where the y offset is
 * thus half the slope), using slope and the fractional component to see
 * when motion along the shorter axis is necessary.  Since we assume that
 * vision is not blocked by "brushing" the corner of any grid, we must do
 * some special checks to avoid testing grids which are "brushed" but not
 * actually "entered".
 */

int dr1_los(int y1, int x1, int y2, int x2, dr1Map *map);


#endif /* __DR1UTIL__H */
