#include <SDL.h>
#include <SDL_ttf.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "text.h"
#include "common.h"
#include "race.h"

char* race_name[] = { 
	"HUMAN",
	"ELF",
	"HOBBIT",
	"HALFORC",
	"DWARF"
    };

char* class_name[] = {
	"INVALID",
	"MU",
	"CLERIC",
	"FIGHTER",
	"THIEF",
	/* not implemented below here */
	"ILLUSIONIST",
	"DRUID",
	"ASASSIN",
	"PALADIN",
	"RANGER",
	"MONK"
    };

static struct {
    dr1Text dragons;
    dr1Text reach;
    dr1Text name;
    dr1Text race;
    dr1Text class_level;
    dr1Text sex;
    dr1Text experience;
    dr1Text hits;
    dr1Text fatigue;
    dr1Text stats_labels[6];
    dr1Text stats_values[6];
    dr1Text weapon;
    dr1Text gauche;
    dr1Text armor;
    dr1Text encumbrance;
    dr1Text attack_action;
    dr1Text attack_result;
    dr1Text enemy;
    dr1Text defend_action;
    dr1Text defend_result;
    dr1Text command_echo;
    dr1Text _scrollmsg[4];
} _text;

static char* _msgattr[] = { "Str:", "Int:", "Wis:", "Dex:", "Con:", "Cha:" };

#define TITLE_PTSIZE 28
#define NAME_PTSIZE 24
#define DEFAULT_PTSIZE 18
#define MESSAGE_PTSIZE 18

/* left column status box -- half line feed vertical position macro */
#define lpos(ln) (61 + (ln) * 11)
#define XPOS 20

/* message status box -- half line feed vertical position macro */
#define mpos(ln) (screen->h - 103 + (ln) * 10)


int dr1Text_setInfo( dr1Text *buf, char *string, int ptsize, int x, int y, enum Position pos) 
{
    static TTF_Font *font;
    static SDL_Surface *temp;
    static SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
    static SDL_Color black = { 0, 0, 0, 0 };
    if (ptsize <= 0) ptsize = DEFAULT_PTSIZE;

    strncpy( buf->text, string, sizeof(buf->text));

    if (buf->text[0] == 0) {
        /* null string */
	buf->textbuf = NULL;
	return 0;
    }

    /* Open the font file with the requested point size */
    /* font = TTF_OpenFont("../font/Wizard__.ttf", ptsize); /**/
    /* font = TTF_OpenFont("../font/American.ttf", ptsize); /**/
    font = TTF_OpenFont("../font/MORPHEUS.TTF", ptsize);
    if (!font) {
	    fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
				    ptsize, "Wizard__.ttf", SDL_GetError());
	    exit(2);
    }
    assert(font);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

    /* Render the text */
    buf->textbuf = TTF_RenderText_Shaded(font, string, black, white);
    assert(buf->textbuf);
    switch (pos) {
	case ANCHOR_CTRLEFT:
	case ANCHOR_TOPLEFT:
	case ANCHOR_BOTLEFT:
	    buf->dstrect.x = x;
	    break;
	case ANCHOR_CENTER:
	case ANCHOR_TOPCENTER:
	case ANCHOR_BOTCENTER:
	    buf->dstrect.x = x - buf->textbuf->w/2;
	    break;
	case ANCHOR_CTRRIGHT:
	case ANCHOR_TOPRIGHT:
	case ANCHOR_BOTRIGHT:
	    buf->dstrect.x = x - buf->textbuf->w;
	    break;
    }
    switch (pos) {
	case ANCHOR_CTRLEFT:
	case ANCHOR_CENTER:
	case ANCHOR_CTRRIGHT:
	    buf->dstrect.y = y - buf->textbuf->h / 2;
	    break;
	
	case ANCHOR_TOPLEFT:
	case ANCHOR_TOPCENTER:
	case ANCHOR_TOPRIGHT:
	    buf->dstrect.y = y;
	    break;

	case ANCHOR_BOTLEFT:
	case ANCHOR_BOTCENTER:
	case ANCHOR_BOTRIGHT:
	    buf->dstrect.y = y - buf->textbuf->h;
	    break;
    }
    buf->dstrect.w = buf->textbuf->w;
    buf->dstrect.h = buf->textbuf->h;
    
/*    printf("Font is generally %d big, and string is %hd big\n",
	    TTF_FontHeight(font), buf->textbuf->h); /**/

    /* Set the text colorkey and convert to display format */
    if ( SDL_SetColorKey(buf->textbuf, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0) < 0 ) {
	fprintf(stderr, "Warning: Couldn't set text colorkey: %s\n",
		SDL_GetError());
    }
    temp = SDL_DisplayFormat(buf->textbuf);
    if ( temp != NULL ) {
	    SDL_FreeSurface(buf->textbuf);
	    buf->textbuf = temp;
    }
    TTF_CloseFont(font);
    return 0;
}

void
dr1Text_infoMessage( char *s, SDL_Surface *screen) {
    static pthread_mutex_t infoMessageMutex = PTHREAD_MUTEX_INITIALIZER;
    int i;

    pthread_mutex_lock( &infoMessageMutex);
    for (i = 0; i < 3; i++) {
	_text._scrollmsg[i].textbuf = _text._scrollmsg[i+1].textbuf;
    }
    dr1Text_setInfo( &_text._scrollmsg[ i], s, 18, 140, mpos(i<<1), ANCHOR_TOPLEFT);
    pthread_mutex_unlock( &infoMessageMutex);
}

int dr1Text_showInfo( SDL_Surface *screen, dr1Text *txt) {
    if (!txt->textbuf) return 0;
    SDL_BlitSurface(txt->textbuf, NULL, screen, &txt->dstrect);
    return 0;
}

int dr1Text_setPlayer( dr1Player *p) {
    int i;
    char buf[80];
    common.player = *p;
    dr1Text_setInfo( &_text.name, common.player.name,
	    NAME_PTSIZE, XPOS, lpos(2), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.race, race_name[ common.player.race],
	    DEFAULT_PTSIZE, XPOS, lpos(5), ANCHOR_TOPLEFT);
    sprintf(buf, "%s/%d", class_name[ common.player.class],
            common.player.level);
    dr1Text_setInfo( &_text.class_level, buf,
	    DEFAULT_PTSIZE, XPOS, lpos(7), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.sex, 
            (common.player.sex == DR1R_MALE) ? "Male":"Female", 
	    DEFAULT_PTSIZE, XPOS, lpos(9), ANCHOR_TOPLEFT);
    sprintf(buf, "x: %ld", common.player.xp);
    dr1Text_setInfo( &_text.experience, buf,
	    DEFAULT_PTSIZE, XPOS, lpos(11), ANCHOR_TOPLEFT);
    sprintf(buf, "Hits: %d/%d", common.player.hp - common.player.wounds,
	    common.player.hp);
    dr1Text_setInfo( &_text.hits, buf,
	    DEFAULT_PTSIZE, XPOS, lpos(13), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.fatigue, "Fat: 100%", 
	    DEFAULT_PTSIZE, XPOS, lpos(15), ANCHOR_TOPLEFT);

    /* stats */
    for (i=0; i<6; i++) {
        int *sp = dr1Attr_estatptr( &common.player.curr_attr, i);
	int estr = common.player.curr_attr.estr;

        if (i == 0 && estr) {
	    if (estr == 100) estr = 0;
	    sprintf(buf, "%d/%02d", *sp, common.player.curr_attr.estr);
	} else {
	    sprintf(buf, "%d", *sp);
	}
	dr1Text_setInfo( &_text.stats_values[i], buf,
		DEFAULT_PTSIZE, 65, lpos(17+(i<<1)), ANCHOR_TOPLEFT);
    }

    /* equip */
    if (common.player.weapon) {
	dr1Text_setInfo( &_text.weapon, common.player.weapon->super.name,
	    DEFAULT_PTSIZE, XPOS, lpos(29), ANCHOR_TOPLEFT);
    }
    if (common.player.gauche) {
	dr1Text_setInfo( &_text.gauche, common.player.gauche->name,
	    DEFAULT_PTSIZE, XPOS, lpos(31), ANCHOR_TOPLEFT);
    }
    if (common.player.armor) {
	dr1Text_setInfo( &_text.armor, common.player.armor->super.name,
	    DEFAULT_PTSIZE, XPOS, lpos(33), ANCHOR_TOPLEFT);
    }
    dr1Text_setInfo( &_text.encumbrance, "Carry: 135#", 
	    DEFAULT_PTSIZE, XPOS, lpos(35), ANCHOR_TOPLEFT);
    return 0;
}

int dr1Text_init( SDL_Surface *screen) {
    int i;
    /* title */
    dr1Text_setInfo( &_text.dragons, 
	    "Dragon's", TITLE_PTSIZE, 70, 20, ANCHOR_TOPCENTER);
    dr1Text_setInfo( &_text.reach, 
	    "Reach", TITLE_PTSIZE, 70, 45, ANCHOR_TOPCENTER);

    /* character */
    dr1Text_setInfo( &_text.name, "",
	    NAME_PTSIZE, XPOS, lpos(2), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.race, race_name[ common.player.race],
	    DEFAULT_PTSIZE, XPOS, lpos(5), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.class_level, "",
	    DEFAULT_PTSIZE, XPOS, lpos(7), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.sex, "",
	    DEFAULT_PTSIZE, XPOS, lpos(9), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.experience, "",
	    DEFAULT_PTSIZE, XPOS, lpos(11), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.hits, "",
	    DEFAULT_PTSIZE, XPOS, lpos(13), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.fatigue, "Fat:",
	    DEFAULT_PTSIZE, XPOS, lpos(15), ANCHOR_TOPLEFT);

    /* stats */
    for (i=0; i<6; i++) {
	dr1Text_setInfo( &_text.stats_labels[i], _msgattr[i], 
		DEFAULT_PTSIZE, 60, lpos(17+(i<<1)), ANCHOR_TOPRIGHT);
	dr1Text_setInfo( &_text.stats_values[i], "",
		DEFAULT_PTSIZE, 65, lpos(17+(i<<1)), ANCHOR_TOPLEFT);
    }

    /* equip */
    dr1Text_setInfo( &_text.weapon, "",
	    DEFAULT_PTSIZE, XPOS, lpos(29), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.gauche, "",
	    DEFAULT_PTSIZE, XPOS, lpos(31), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.armor, "",
	    DEFAULT_PTSIZE, XPOS, lpos(33), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.encumbrance, "",
	    DEFAULT_PTSIZE, XPOS, lpos(35), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.attack_action, "",
	    DEFAULT_PTSIZE, XPOS, lpos(37), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.attack_result, "",
	    DEFAULT_PTSIZE, XPOS, lpos(39), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.enemy, "",
	    DEFAULT_PTSIZE, XPOS, lpos(41), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.defend_action, "",
	    DEFAULT_PTSIZE, XPOS, lpos(43), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.defend_result, "",
	    DEFAULT_PTSIZE, XPOS, lpos(45), ANCHOR_TOPLEFT);
    dr1Text_setInfo( &_text.command_echo, "",
	    DEFAULT_PTSIZE, XPOS, lpos(47), ANCHOR_TOPLEFT);
    
    for ( i=0; i<4; i++) {
	dr1Text_setInfo( &_text._scrollmsg[i], "*", 
		MESSAGE_PTSIZE, 140, mpos(i<<1), ANCHOR_TOPLEFT);
    }
    return 0;
}

int dr1Text_show( SDL_Surface *screen) {
    int i;
    dr1Text_showInfo( screen, &_text.dragons);
    dr1Text_showInfo( screen, &_text.reach);
    dr1Text_showInfo( screen, &_text.name);
    dr1Text_showInfo( screen, &_text.race);
    dr1Text_showInfo( screen, &_text.class_level);
    dr1Text_showInfo( screen, &_text.sex);
    dr1Text_showInfo( screen, &_text.experience);
    dr1Text_showInfo( screen, &_text.hits);
    dr1Text_showInfo( screen, &_text.fatigue);
    for (i=0; i<6; i++) {
	dr1Text_showInfo( screen, &_text.stats_labels[ i]);
	dr1Text_showInfo( screen, &_text.stats_values[ i]);
    }
    dr1Text_showInfo( screen, &_text.weapon);
    dr1Text_showInfo( screen, &_text.gauche);
    dr1Text_showInfo( screen, &_text.armor);
    dr1Text_showInfo( screen, &_text.encumbrance);
    dr1Text_showInfo( screen, &_text.attack_action);
    dr1Text_showInfo( screen, &_text.attack_result);
    dr1Text_showInfo( screen, &_text.enemy);
    dr1Text_showInfo( screen, &_text.defend_action);
    dr1Text_showInfo( screen, &_text.defend_result);
    dr1Text_showInfo( screen, &_text.command_echo);
    for (i=0; i<4; i++) {
	dr1Text_showInfo( screen, &_text._scrollmsg[i]);
    }
    return 0;
}

