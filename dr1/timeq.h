#ifndef __DR1TIMEQ__H
#define __DR1TIMEQ__H
#ifndef __DR1PLAYER__H
#   include "player.h"
#endif

/*-------------------------------------------------------------------
 * dr1TimeQ
 *
 *   This structure maintains a list of upcoming events ordered
 *   by the time at which they should occur.  Temporary magical
 *   effects like a spell or potion wearing off are good examples
 *   of events that should be placed on the queue.
 *
 *   Events can also be removed aynchronously from the queue so
 *   that dispel magic can be done correctly for example.
 */
struct dr1TimeQ_node;
typedef struct {
    struct dr1TimeQ_node *head;
} dr1TimeQ;


/*-------------------------------------------------------------------
 * dr1Event_fp: callback function pointer
 *   
 *   Parameters:
 *      time	Time at expiration of the timer
 *      ctx	Pointer to a context structure
 *
 *   Returns:
 *      0	Event completed
 *      +N      Requeue the event for N seconds from now
 */

struct dr1TimeQ_ctx;
typedef int (*dr1Event_fp)( long time, struct dr1TimeQ_ctx* ctx);

/*-------------------------------------------------------------------
 * dr1TimeQ_ctx: context structure
 *
 *   Fields:
 *      player	The player affected by the event
 *      user	Additional user context
 */

typedef struct dr1TimeQ_ctx {
    dr1Player *player;
    void *user;
} dr1TimeQ_ctx;

/*-------------------------------------------------------------------
 * dr1TimeQ_insert
 *    
 *    Parameters:
 *       tq	The TimeQ to be added to
 *       time   Time of the event
 *       event	The callback function
 *       ctx	Context for the callback function
 */

void dr1TimeQ_insert( 
	dr1TimeQ *tq, long time, dr1Event_fp fn, 
	dr1TimeQ_ctx ctx
    );

/*-------------------------------------------------------------------
 * dr1TimeQ_delete
 *    
 *    Parameters:
 *       tq	The TimeQ to be changed
 *       ctx	Context pointer previously added
 *
 *    Return:
 *       On success returns a pointer to the dequeued node
 *	 On failure returns NULL.
 */

struct dr1TimeQ_node* dr1TimeQ_delete( dr1TimeQ *tq, dr1TimeQ_ctx ctx);

#endif /* _DR1SET__H */
