GFXDIR="../gfx"
CFLAGS=`sdl-config --cflags` -I/usr/local/include/SDL

test: test.o map.o util.o
	gcc -g test.o map.o util.o -o test `sdl-config --libs`  -lSDL_ttf -lttf

test2: showfont.o
	gcc showfont.o -o test2 `sdl-config --libs` -lSDL_ttf -lttf

.c.o:
	gcc -g -c -DGFXDIR='${GFXDIR}' ${CFLAGS} $<

