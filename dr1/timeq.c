#include <stdlib.h>
#include "player.h"
#include "timeq.h"

/*-------------------------------------------------------------------*/
struct dr1TimeQ_node {
    long time;
    dr1event_fp callback;
    dr1TimeQ_ctx context;
    struct dr1TimeQ_node *next;
} dr1TimeQ_node;

static int ctx_equal( dr1TimeQ_ctx *a, dr1TimeQ_ctx *b) {
    int res;
    res = (a->player == b->player) && (a->user == b->user);
    return res;
}

/*-------------------------------------------------------------------
 * dr1TimeQ_insert
 *    
 *    Parameters:
 *       tq	The TimeQ to be added to
 *       time   Time of the event
 *       event	The callback function
 *       ctx	Context for the callback function
 */

static void dr1TimeQ_insertnode( 
	dr1TimeQ *tq, struct dr1TimeQ_node *node
    ) 
{
    struct dr1TimeQ_node *lp, *p; 
    
    lp = p = NULL;

    for (p=tq->head; lp!=NULL; lp=p, p=p->next) {
	if (p == NULL || p->time > node->time) {
	    /* insert here */

	    if (lp == NULL) {
		tq->head = node;
	    } else {
		lp->next = node;
	    }

            node->next = p;
	    break;
	} /* if */
    } /* for */
}

void dr1TimeQ_insert( 
	dr1TimeQ *tq, long time, dr1event_fp event, 
	dr1TimeQ_ctx ctx
    ) 
{
    struct dr1TimeQ_node *node;
    
    node = malloc( sizeof(dr1TimeQ_node));
    node->time = time;
    node->callback = event;
    node->context = ctx;

    dr1TimeQ_insertnode( tq, node);
}

/*-------------------------------------------------------------------
 * dr1TimeQ_delete
 *    
 *    Parameters:
 *       tq	The TimeQ to be added to
 *       ctx	Context pointer previously added
 *
 *    Return:
 *       On success returns a pointer to the dequeued node 
 *	 On failure returns NULL.
 */

struct dr1TimeQ_node* dr1TimeQ_delete( dr1TimeQ *tq, dr1TimeQ_ctx ctx) {
    struct dr1TimeQ_node *node, *p, *lp; 
    node = lp = p = NULL;

    for (p=tq->head; p!=NULL; lp=p, p=p->next) {
	if (ctx_equal( &p->context, &ctx)) {
	    /* delete here */
	    node=p;

	    if (lp == NULL) {
		tq->head = p->next;
	    } else {
		lp->next = p->next;
	    }
	    p->next = NULL;
	    break;
	} /* if */
    } /* for */
    return node;
}

/*-------------------------------------------------------------------
 * dr1TimeQ_newtime
 *    
 *    Parameters:
 *       tq	Time Q to modify
 *       now	Current time
 *
 */

void dr1TimeQ_newtime( dr1TimeQ* tq, int now) {
    struct dr1TimeQ_node *n;
    int ffwd;

    if (!tq->head) return;
    
    while (tq->head->time <= now) {
	n = dr1TimeQ_delete( tq, tq->head->context);
	ffwd = (*(n->callback))(n->time, &n->context);
	if (n == 0) {
	    free( n);
	} else {
	    n->time += ffwd;
	    dr1TimeQ_insertnode( tq, n);
	}
    }
}
