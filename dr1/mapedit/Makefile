GFXDIR="../gfx"
CFLAGS=-I.. \
	`sdl-config --cflags` -I/usr/local/include/SDL \
	`freetype-config --cflags`
#LIBS=   `sdl-config --libs` -L/usr/local/lib -lSDL_ttf \
#	`freetype-config --libs` \
#	-lxml2 -L.. -lefence -l../lib/libdr1.a
LIBS=   `sdl-config --libs` -L/usr/local/lib -lSDL_ttf \
	`freetype-config --libs` \
	-lxml2 ../lib/libdr1.a

test: test.o glyphset.o
	gcc -g test.o glyphset.o -o test ${LIBS}
#	gcc -g test.o glyphset.o ../xdrxml.o -o test `sdl-config --libs`  -lSDL_ttf -lttf -lxml2 -L.. -lefence

test2: showfont.o
	gcc showfont.o -o test2 `sdl-config --libs` -lSDL_ttf -lttf

.c.o:
	gcc -g -c -DGFXDIR='${GFXDIR}' ${CFLAGS} $<

