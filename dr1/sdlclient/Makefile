GFXDIR="../gfx"
CFLAGS=-I../lib -I.. \
	`sdl-config --cflags` -I/usr/local/include/SDL \
	`freetype-config --cflags`
LIBS=   `sdl-config --libs` -L/usr/local/lib -lSDL_ttf \
	`freetype-config --libs` \
	-lxml2 ../lib/libdr1.a -L. -lefence
LIBS=   `sdl-config --libs` -L/usr/local/lib -lSDL_ttf \
	`freetype-config --libs` \
	-lxml2 ../lib/libdr1.a

dr1: dr1.o util.o glyphset.o text.o comm.o common.o
	gcc -g dr1.o util.o glyphset.o text.o comm.o common.o -o dr1 ${LIBS}

test: test.o util.o glyphset.o text.o
	gcc -g test.o util.o glyphset.o text.o -o test ${LIBS}

test2: showfont.o
	gcc showfont.o -o test2 `sdl-config --libs` -lSDL_ttf -lttf

.c.o:
	gcc -Wall -g -c -DGFXDIR='${GFXDIR}' ${CFLAGS} $<

