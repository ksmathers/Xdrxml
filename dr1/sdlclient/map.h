#include <SDL.h>

extern SDL_Surface *dr1_npcs2;
extern SDL_Surface *dr1_dung1;
extern SDL_Surface *dr1_dung3;
extern SDL_Surface *dr1_castle1;
extern SDL_Surface *dr1_objects1;
extern SDL_Surface *dr1_objects2;

typedef struct dr1Glyph dr1Glyph;
struct dr1Glyph {
    SDL_Surface *src;
    int r;
    int c;
    int anim:1;
    int wall:1;
    int door:1;
    int startinvisible:1;
};

typedef struct dr1MapGraphic dr1MapGraphic;
struct dr1MapGraphic {
    int code;
    int nglyphs;
    dr1Glyph *glyph; 
    int start:1;
    int light:1;	/* ambient light */
};


typedef struct dr1MapGrid dr1MapGrid;
struct dr1MapGrid {
    dr1MapGraphic *graphic;
    int seen:1;
    int invisible:1;		/* top glyph is invisible */
    int open:1;			/* top glyph is open */
};

typedef struct dr1Map dr1Map;
struct dr1Map {
    int ngraphics;
    dr1MapGraphic *graphics;
    int xsize; 
    int ysize;
    dr1MapGrid *grid;
    int startrow;
    int startcol;
};

dr1Map readmap( char *fname);
