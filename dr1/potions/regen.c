#include "player.h"
#include "dice.h"
#include "timeq.h"

static struct regen_data {
    int rounds_remaining;
}

void dr1pregen_quaff( dr1player* p) {
    dr1timeq_ctx *ctx = calloc( sizeof(dr1timeq_ctx));
    struct regen_data *data = calloc( sizeof(regen_data));
    data->time_remaining = dr1dice_roll("d8");
    ctx->player = p;
    ctx->user = data;
    dr1timeq_insert( now + 60, dr1pregen_event, ctx);
    dr1pregen_event( p);
}

int dr1pregen_event( long time, dr1timeq_ctx* ctx) {
    dr1player *p = ctx->player;
    struct regen_data *data = (struct regen_data *)ctx->user;

    if (p->hp > p->full_hp) return;
    p->hp += 1;
    if (p->hp > p->full_hp) p->hp=p->full_hp;

    data->rounds_remaining -= 60;
    if (data->time_remaining < 0) {
        /* all done.  clean up and leave. */
	free(data);
	free(user);
	return 0;
    }
    /* requeue */
    return 60;
}
