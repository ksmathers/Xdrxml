#include <SDL.h>

extern SDL_Surface *dr1_npcs2;
extern SDL_Surface *dr1_dung1;
extern SDL_Surface *dr1_dung3;
extern SDL_Surface *dr1_castle1;
extern SDL_Surface *dr1_objects1;

typedef struct dr1Glyph dr1Glyph;
struct dr1Glyph {
    SDL_Surface *src;
    int anim;
    int r;
    int c;
};

typedef struct dr1MapGraphic dr1MapGraphic;
struct dr1MapGraphic {
    int code;
    int nglyphs;
    dr1Glyph *glyph; 
};

typedef struct dr1MapGrid dr1MapGrid;
struct dr1MapGrid {
    int xsize;
    int ysize;
    dr1MapGraphic **graphic;
};

typedef struct dr1Map dr1Map;
struct dr1Map {
    int ngraphics;
    dr1MapGraphic *graphics;
    dr1MapGrid grid;
};

dr1Map readmap( char *fname);