#include <string.h>
#include <assert.h>
#include "combatv.h"
#include "player.h"
#include "monster.h"
#include "dice.h"

int attack( dr1Player *p, dr1Monster *m, int c, char **v) {
    int fleeing = 0;
    int p_thac0 = dr1Player_thac0( p);
    int m_thac0 = dr1Monster_thac0( m);
    int tohit, roll, crit;
    int mul=1;
    int i;

    if (!c) return -1;
    if (!strcasecmp( v[0], "run")) {
	fleeing = 1;
    }


    /* Player swings */
    if (!fleeing) {
	crit = 0;
	tohit = p_thac0 + m->type->ac;
	roll = dr1Dice_roll("d20");
	if ( roll == 20) {
	    /* natural 20 gets a bonus */
	    if ( tohit>20) roll += 4;
	    else crit=dr1Dice_roll("d12");
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		mul = 2;
		break;
	    case 12: 
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    int dam;

	    dam = dr1Dice_roll( p->weapon->damage);
	    printf("Hit! %d Damage.\n", dam);
	    m->hp -= dam;
	} else {
	    printf("Swish!\n");
	}
    } /* if fleeing */

    /* Monster swings */
    for (i=0; i < m->type->nattacks; i++) {
	assert(m->type->damage[i]);
	/* FIXME */
	tohit = m_thac0 + dr1Player_ac( p, 0, m->type->damage[i]->ranged, 0);
	roll = dr1Dice_roll("d20");
	if ( roll == 20) {
	    /* natural 20 gets a bonus */
	    if ( tohit>20) roll += 4;
	    else crit=dr1Dice_roll("d12");
	}

	switch (crit) {
	    case 7:
	    case 8:
	    case 9:
	    case 10:
	    case 11: 
		mul = 2;
		break;
	    case 12: 
		mul = 3;
		break;
	}

	if (roll >= tohit) {
	    int dam;
	    dam = dr1Dice_roll( m->type->damage[i]->damage);
	    printf("Struck Thee! %d damage.\n", dam);
	    m->hp -= dam;
	} else {
	    printf("Swish!\n");
	}
    } /* for */

    return 0;
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
void dr1Combatv_showPage( dr1Player *p, dr1Monster *m) {
    char cmd[80];
    char *cmds[10];
    do {
        int i;
        printf("-------------------------------------------------------\n");
	printf("Player: %-20s    Hits: %d/%d\n", p->name, p->hp, p->full_hp);
	printf("Monster: %-20s   Damage: %d\n", m->type->name, m->full_hp - m->hp);
        printf("(attack, equip, run)\n");
	printf("Command: ");

	gets( cmd);

        i=0;
	cmds[0] = strtok( cmd, " \t\n");
	if (!cmds[0]) continue;
	while ((cmds[++i] = strtok( NULL, " \t\n")) != 0 && i<10 );

        if (!strcasecmp( cmds[0], "attack")) attack( p, m, i, cmds);
        else if (!strcasecmp( cmds[0], "run")) attack( p, m, i, cmds);
	else {
	    printf("Unknown command %s.\n", cmds[0]);
	}

	if (p->hp < 0) {
	    printf("Thou'rt slain.  Donai nais requiem.  Resquiat en pacem.\n");
	    break;
	}

	if (m->hp < 0) {
	    printf("The beast is slain.  Thus falls the hammer of god upon the infidel.\n");
	    printf("In nomen de Padre, facilitus Domine.\n");
	    p->xp += m->type->xp;
	    break;
	}
    } while (strcasecmp( cmds[0], "run")); 

}


