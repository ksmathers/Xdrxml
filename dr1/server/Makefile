OBJS=	    server.o \
	    barter.o \
	    playerv.o \
	    smithy.o \
	    ttype.o tanner.o wright.o \
	    context.o glyphset.o \
	    controller.o \
	    merchant.o apothecary.o 

.c.o:
	gcc -c -g -Wall -Wno-comment -I../lib -I. $< -o $@

#	gcc -c -g -Wall -Wno-comment -Wno-multichar -I. $< -o $@

all: dr1server

dr1server: ${OBJS}
	gcc -g -Wl,-warn-common -Wl,-Map=dr1server.map -odr1server \
	${OBJS} \
	../lib/libdr1.a -lxml2 -lm

depend:
	makedepend -I. \
		-I/usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include *.c

test: money.o test.o dice.o
	gcc -g test.o money.o dice.o -o test -lm

tags:
	ctags *.h *.c

clean:
	rm *.o

test.o: test.c
dice.o: dice.c
player.o: player.c
money.o: money.c
potion/regen.o: potion/regen.c
