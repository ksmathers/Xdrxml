OBJS=	    server.o \
	    barter.o itemset.o item.o attr.o registry.o \
	    itemreg.o playerv.o race.o class.o \
	    smithy.o weapon.o monster.o armor.o \
	    ttype.o tanner.o wright.o location.o \
	    potions/heal.o context.o glyphset.o \
	    controller.o dice.o player.o money.o \
	    merchant.o apothecary.o \
	    lib/libdr1.a

.c.o:
	gcc -c -g -Wall -Wno-comment -I. $< -o $@

#	gcc -c -g -Wall -Wno-comment -Wno-multichar -I. $< -o $@

all: subdirs dr1server

subdirs:
	cd gfx/24x35 ; make
	cd lib ; make
	cd mapedit ; make
#	cd sdlclient ; make

dr1server: ${OBJS}
	gcc -g -Wl,-warn-common -Wl,-Map=dr1server.map -odr1server \
	${OBJS} \
	-lxml2 -lm lib/libdr1.a

depend:
	makedepend -I. \
		-I/usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include *.c

test: money.o test.o dice.o
	gcc -g test.o money.o dice.o -o test -lm

tags:
	ctags *.h *.c

clean:
	rm *.o */*.o

test.o: test.c
dice.o: dice.c
player.o: player.c
money.o: money.c
potion/regen.o: potion/regen.c
