#ifndef __DR1COMM__H
#define __DR1COMM__H

/*-------------------------------------------------------------------
 * dr1
 *
 *    The structure ...
 */


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

void* comm_main( void* iparm);
int sendCommand( char key);

/*-------------------------------------------------------------------
 * doConnect
 *
 *    The method doConnect opens a connection to the game server using
 *    information stored in the 'common' global set.   doConnect() is 
 *    called as a result of the login or newplayer button clicks on
 *    the login dialog.
 *
 *  PARAMETERS:
 *
 *  RETURNS:
 *
 *  SIDE EFFECTS:
 */
int doConnect( void);

#endif /* __DR1__H */
