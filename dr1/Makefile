.c.o:
	gcc -c -g -Wall -Wno-comment -I. $< -o $@

#	gcc -c -g -Wall -Wno-comment -Wno-multichar -I. $< -o $@

all: subdirs dr1server

subdirs:
	cd gfx/24x35 ; make
	cd lib ; make
	cd mapedit ; make
	cd sdlclient ; make

dr1server: server.o money.o merchant.o apothecary.o dice.o player.o \
	    barter.o itemset.o item.o attr.o registry.o \
	    itemreg.o playerv.o race.o class.o \
	    smithy.o weapon.o monster.o combatv.o armor.o \
	    ttype.o tanner.o wright.o location.o qio.o \
	    potions/heal.o context.o town.o glyphset.o
	gcc -g -Wl,-warn-common -Wl,-Map=dr1server.map -odr1server server.o money.o merchant.o \
	apothecary.o dice.o \
	player.o barter.o itemset.o item.o attr.o registry.o \
	itemreg.o playerv.o race.o class.o \
	smithy.o weapon.o monster.o combatv.o armor.o \
	ttype.o tanner.o wright.o location.o qio.o \
	context.o town.o glyphset.o \
	potions/heal.o -lxml2 -lm lib/libdr1.a

main: main.o money.o merchant.o apothecary.o dice.o player.o \
	    barter.o itemset.o item.o attr.o registry.o \
	    itemreg.o xdrxml.o playerv.o race.o class.o \
	    smithy.o weapon.o monster.o combatv.o armor.o \
	    ttype.o tanner.o wright.o location.o \
	    potions/heal.o 
	gcc -g -Wl,-warn-common -odr1 main.o money.o merchant.o apothecary.o dice.o \
	player.o barter.o itemset.o item.o attr.o registry.o \
	itemreg.o xdrxml.o playerv.o race.o class.o \
	smithy.o weapon.o monster.o combatv.o armor.o \
	ttype.o tanner.o wright.o location.o \
	potions/heal.o -lxml2 -lm

#xtest: xtest.o xdrxml.o
#	gcc -g -oxtest xtest.o xdrxml.o -lxml2

xtest2: xtest.o xdrxml.o
	gcc -g -oxtest xtest.o xdrxml.o dice.o money.o registry.o item.o itemreg.o armor.o weapon.o potions/heal.o -lxml2

xtest3: xtest.o xdrxml.o
	gcc -g -oxtest xtest.o xdrxml.o dice.o money.o registry.o item.o itemreg.o armor.o weapon.o potions/heal.o class.o race.o location.o itemset.o -lxml2
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
