GFXDIR="../gfx/24x35"

test: test.o map.o util.o
	gcc -g test.o map.o util.o -o test `sdl-config --libs` 

test2: test2.o
	gcc test2.o -o test2 `sdl-config --libs`

.c.o:
	gcc -g -c -DGFXDIR='${GFXDIR}' `sdl-config --cflags` $<

