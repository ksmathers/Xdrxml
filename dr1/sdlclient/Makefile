GFXDIR="../gfx"
CFLAGS=-I.. \
	`sdl-config --cflags` -I/usr/local/include/SDL \
	`freetype-config --cflags`
LIBS=   `sdl-config --libs` -L/usr/local/lib -lSDL_ttf \
	`freetype-config --libs` \
	-lxml2 ../lib/libdr1.a

test: test.o util.o glyphset.o text.o
	gcc -g test.o util.o glyphset.o text.o -o test ${LIBS}

test2: showfont.o
	gcc showfont.o -o test2 `sdl-config --libs` -lSDL_ttf -lttf

.c.o:
	gcc -g -c -DGFXDIR='${GFXDIR}' ${CFLAGS} $<

