.c.o:
	gcc -c -g -Wall -Wno-comment -Wno-multichar -I. $< -o $@

main: main.o money.o merchant.o apothecary.o dice.o player.o \
	    barter.o itemset.o item.o attr.o registry.o \
	    itemreg.o xdrasc.o playerv.o race.o class.o \
	    smithy.o weapon.o \
	    potions/heal.o 
	gcc -g main.o money.o merchant.o apothecary.o dice.o \
	player.o barter.o itemset.o item.o attr.o registry.o \
	itemreg.o xdrasc.o playerv.o race.o class.o \
	smithy.o weapon.o \
	potions/heal.o -lm

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
