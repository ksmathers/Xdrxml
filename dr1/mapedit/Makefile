GFXDIR="../gfx"
CFLAGS=`sdl-config --cflags` -I/usr/local/include/SDL -I..

test: test.o map.o glyphset.o
	gcc -g test.o map.o glyphset.o ../xdrxml.o -o test `sdl-config --libs`  -lSDL_ttf -lttf -lxml2
#	gcc -g test.o map.o glyphset.o ../xdrxml.o -o test `sdl-config --libs`  -lSDL_ttf -lttf -lxml2 -L.. -lefence

test2: showfont.o
	gcc showfont.o -o test2 `sdl-config --libs` -lSDL_ttf -lttf

.c.o:
	gcc -g -c -DGFXDIR='${GFXDIR}' ${CFLAGS} $<

